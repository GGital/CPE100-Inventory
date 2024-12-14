#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "report.h"
#include "cart.h"
#include "CRUD.h"
#include "beautiful_cli.h"

#define MAX_PRODUCTS 500
#define MAX_TRANSACTIONS 1000

void Parse_transaction(const char *transaction_txt, const char *date_filter, SoldProduct sold_products[], int *product_count, double *total_revenue_after_discount)
{
    FILE *file = fopen(transaction_txt, "r");
    if (!file)
    {
        printf("Error: Unable to open file %s.\n", transaction_txt);
        return;
    }

    char line[1000];
    *product_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (*product_count >= MAX_TRANSACTIONS)
        {
            printf("Maximum transaction limit reached.\n");
            break;
        }

        // Remove trailing newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        char customer_name[50], used_coupon[50], datetime[25], products_section[500];
        double total_price;

        // Use a more flexible parsing approach
        char *token = strtok(line, "|");
        if (!token)
            continue;
        strncpy(customer_name, token, sizeof(customer_name) - 1);
        customer_name[sizeof(customer_name) - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strncpy(used_coupon, token, sizeof(used_coupon) - 1);
        used_coupon[sizeof(used_coupon) - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token)
            continue;
        total_price = atof(token);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strncpy(datetime, token, sizeof(datetime) - 1);
        datetime[sizeof(datetime) - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strncpy(products_section, token, sizeof(products_section) - 1);
        products_section[sizeof(products_section) - 1] = '\0';

        // Extract date from datetime
        char date[11];
        strncpy(date, datetime + 1, 10); // Extract "YYYY-MM-DD"
        date[10] = '\0';

        // printf("%sd%s\n", date, date_filter);
        //  Filter by date
        if (strcmp(date, date_filter) != 0)
            continue;

        *(total_revenue_after_discount) += total_price;

        // printf("pass\n");
        // Parse products_section (e.g., "products: [4:20]")
        char *product_list = strstr(products_section, "[");
        if (product_list)
        {
            product_list++; // Skip '['
            char *end_bracket = strchr(product_list, ']');
            if (end_bracket)
                *end_bracket = '\0'; // Terminate at closing bracket

            token = strtok(product_list, ", ");
            // printf(token);
            while (token && *product_count < MAX_TRANSACTIONS)
            {
                int product_id, quantity;
                if (sscanf(token, "%d:%d", &product_id, &quantity) == 2)
                {
                    sold_products[*product_count].product_id = product_id;
                    sold_products[*product_count].quantity = quantity;
                    (*product_count)++;
                    // printf("%d", *product_count);
                }
                token = strtok(NULL, ", ");
            }
        }
    }

    fclose(file);
}

void Generate_report(const char *transaction_txt, const char *date_filter, double min_price, double max_price, const char *product_name_filter, product prod[], int total_products)
{
    SoldProduct sold_products[MAX_TRANSACTIONS];
    int product_count = 0;
    double total_revenue_after_discount = 0.0;

    Parse_transaction(transaction_txt, date_filter, sold_products, &product_count, &total_revenue_after_discount);

    if (product_count == 0)
    {
        printf("No transactions found for the given date: %s\n", date_filter);
        return;
    }

    ProductSummary summary[MAX_PRODUCTS] = {0};
    int summary_count = 0;
    double total_revenue_before_discount = 0.0;

    for (int i = 0; i < product_count; i++)
    {
        int product_id = sold_products[i].product_id;
        int quantity = sold_products[i].quantity;

        for (int j = 0; j < total_products; j++)
        {
            if (prod[j].id == product_id)
            {
                if (prod[j].price < min_price || prod[j].price > max_price)
                    continue;
                if (product_name_filter[0] != '\0' && strstr(prod[j].name, product_name_filter) == NULL)
                    continue;

                int found = 0;
                for (int k = 0; k < summary_count; k++)
                {
                    if (summary[k].product_id == product_id)
                    {
                        summary[k].total_quantity += quantity;
                        found = 1;
                        break;
                    }
                }

                if (!found && summary_count < MAX_PRODUCTS)
                {
                    summary[summary_count].product_id = product_id;
                    summary[summary_count].total_quantity = quantity;
                    summary_count++;
                }

                total_revenue_before_discount += quantity * prod[j].price;
                break;
            }
        }
    }

    int max_sold_quantity = 0;
    int most_sold_product_id = 0;
    for (int i = 0; i < summary_count; i++)
    {
        if (summary[i].total_quantity > max_sold_quantity)
        {
            max_sold_quantity = summary[i].total_quantity;
            most_sold_product_id = summary[i].product_id;
        }
    }

    char report_filename[100];
    snprintf(report_filename, sizeof(report_filename), "%s_report.txt", date_filter);

    FILE *report_file = fopen(report_filename, "w");
    if (!report_file)
    {
        printf("Error: Unable to create report file %s.\n", report_filename);
        return;
    }

    fprintf(report_file, "Report for Date: %s\n", date_filter);
    fprintf(report_file, "-------------------------\n");
    fprintf(report_file, "Total Revenue Before Discounts: %.2f\n", total_revenue_before_discount);
    fprintf(report_file, "Total Revenue After Discounts: %.2f\n", total_revenue_after_discount);
    fprintf(report_file, "Most Sold Product: %d (Quantity: %d)\n", most_sold_product_id, max_sold_quantity);
    fprintf(report_file, "Product Summary:\n");
    for (int i = 0; i < summary_count; i++)
    {
        for (int j = 0; j < total_products; j++)
        {
            if (prod[j].id == summary[i].product_id)
            {
                fprintf(report_file, "- Product ID: %d, Name: %s, Total Quantity: %d\n", prod[j].id, prod[j].name, summary[i].total_quantity);
                break;
            }
        }
    }

    fclose(report_file);
    printf(ANSI_COLOR_GREEN "Report saved to %s\n" ANSI_COLOR_RESET, report_filename);
}