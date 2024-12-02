#include <stdio.h>
#include <string.h>
#include <time.h>
#include "coupon.h"

int coupon_count = 0;
int max_coupon_index = 0;

void coupon_load(coupon *coupons, const char *coupon_csv)
{
    coupon_count = 0;
    FILE *shared_coupon = fopen(coupon_csv, "r");
    if (!shared_coupon)
    {
        perror("Error opening file");
        return;
    }

    char line[500];
    while (fgets(line, sizeof(line), shared_coupon))
    {
        if (coupon_count == 0) // Skip header
        {
            coupon_count++;
            continue;
        }
        int col = 0;
        char *value = strtok(line, ",");
        while (value)
        {
            if (col == 0)
                sscanf(value, "%d", &coupons[coupon_count - 1].id);
            else if (col == 1)
                strcpy(coupons[coupon_count - 1].name, value);
            else if (col == 2)
                sscanf(value, "%f", &coupons[coupon_count - 1].minimum_cost);
            else if (col == 3)
                sscanf(value, "%f", &coupons[coupon_count - 1].discountRate);
            else if (col == 4)
                sscanf(value, "%d", &coupons[coupon_count - 1].expiry_day);
            else if (col == 5)
                sscanf(value, "%d", &coupons[coupon_count - 1].expiry_month);
            else if (col == 6)
                sscanf(value, "%d", &coupons[coupon_count - 1].expiry_year);

            value = strtok(NULL, ",");
            col++;
        }
        max_coupon_index = (max_coupon_index < coupons[coupon_count - 1].id) ? coupons[coupon_count - 1].id : max_coupon_index;
        coupon_count++;
    }
    fclose(shared_coupon);
}

void coupon_read(coupon *coupons, float min_discount, float max_discount)
{
    int displayed = 0;
    printf("==============================================================================================================\n");
    printf("| %-5s | %-20s | %-15s | %-15s | %-10s |\n",
           "ID", "Name", "Minimum Cost", "Discount Rate", "Expiry Date");
    printf("==============================================================================================================\n");

    for (int i = 1; i < coupon_count; i++)
    {
        if (coupons[i].discountRate >= min_discount && coupons[i].discountRate <= max_discount)
        {
            printf("| %-5d | %-20s | %-15.2f | %-15.2f | %02d/%02d/%04d |\n",
                   coupons[i].id,
                   coupons[i].name,
                   coupons[i].minimum_cost,
                   coupons[i].discountRate,
                   coupons[i].expiry_day,
                   coupons[i].expiry_month,
                   coupons[i].expiry_year);
            displayed = 1;
        }
    }

    if (!displayed)
    {
        printf("| %-92s |\n", "No coupons found in the specified discount rate range.");
    }
    printf("==============================================================================================================\n");
}

// Function to save the updated coupons back to the CSV file
void coupon_save(coupon *coupons, const char *coupon_csv)
{
    FILE *shared_coupon = fopen(coupon_csv, "w");
    if (!shared_coupon)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the header row
    fprintf(shared_coupon, "id,name,minimum_cost,discountRate,expiry_day,expiry_month,expiry_year\n");

    for (int i = 0; i < coupon_count; i++)
    {
        fprintf(shared_coupon, "%d,%s,%.2f,%.2f,%d,%d,%d\n",
                coupons[i].id,
                coupons[i].name,
                coupons[i].minimum_cost,
                coupons[i].discountRate,
                coupons[i].expiry_day,
                coupons[i].expiry_month,
                coupons[i].expiry_year);
    }

    fclose(shared_coupon);
    printf("Coupons saved successfully to %s.\n", coupon_csv);
}

void delete_expired_coupons(coupon *coupons)
{
    // Get the current date
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    int current_day = current_time->tm_mday;
    int current_month = current_time->tm_mon + 1;    // Months are 0-based
    int current_year = current_time->tm_year + 1900; // Years since 1900

    int valid_count = 0;

    for (int i = 0; i < coupon_count; i++)
    {
        // Check if the coupon is expired
        if (coupons[i].expiry_year < current_year ||
            (coupons[i].expiry_year == current_year && coupons[i].expiry_month < current_month) ||
            (coupons[i].expiry_year == current_year && coupons[i].expiry_month == current_month && coupons[i].expiry_day < current_day))
        {
            // Skip expired coupon
            continue;
        }

        // If the coupon is still valid, keep it in the array
        coupons[valid_count++] = coupons[i];
    }

    // Update coupon count to the number of valid coupons
    coupon_count = valid_count;

    printf("Expired coupons removed. %d valid coupons remain.\n", coupon_count);
}

void coupon_create(coupon *coupons, const char *coupon_csv, char name[], float minimum_cost, float discountRate, int expiry_day, int expiry_month, int expiry_year)
{
    max_coupon_index++;
    if (strlen(name) > 0 && minimum_cost >= 0 && discountRate >= 0 &&
        expiry_day > 0 && expiry_day <= 31 &&
        expiry_month > 0 && expiry_month <= 12 &&
        expiry_year >= 1900)
    {
        coupons[max_coupon_index].id = max_coupon_index;
        strcpy(coupons[max_coupon_index].name, name);
        coupons[max_coupon_index].minimum_cost = minimum_cost;
        coupons[max_coupon_index].discountRate = discountRate;
        coupons[max_coupon_index].expiry_day = expiry_day;
        coupons[max_coupon_index].expiry_month = expiry_month;
        coupons[max_coupon_index].expiry_year = expiry_year;

        coupon_count++;
        coupon_save(coupons, coupon_csv);

        printf("Coupon '%s' created successfully!\n", name);
    }
    else
    {
        printf("You must provide valid information for the new coupon.\n");
    }
}