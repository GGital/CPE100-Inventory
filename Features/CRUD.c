#include <stdio.h>
#include <string.h>
#include "CRUD.h"

int product_count = 0;
int max_product_index = 0;

void product_load(product *prod, const char *product_csv)
{
    product_count = 0;
    FILE *shared_product = fopen(product_csv, "r");
    char line[500];
    while (fgets(line, sizeof(line), shared_product))
    {
        int col = 0;
        char *value = strtok(line, ",");
        while (value)
        {
            if (col == 0)
                sscanf(value, "%d", &prod[product_count].id);
            else if (col == 1)
                strcpy(prod[product_count].name, value);
            else if (col == 2)
                strcpy(prod[product_count].description, value);
            else if (col == 3)
                sscanf(value, "%f", &prod[product_count].cost);
            else if (col == 4)
                sscanf(value, "%f", &prod[product_count].price);

            value = strtok(NULL, ",");
            col++;
            max_product_index = (max_product_index < prod[product_count].id) ? prod[product_count].id : max_product_index;
        }
        product_count++;
    }
    fclose(shared_product);
}

void product_create(product *prod, const char *product_csv, char name[], char description[], float cost, float price)
{
    max_product_index++;
    if (strlen(name) > 0 && strlen(description) > 0 && cost >= 0 && price >= 0)
    {
        strcpy(prod[max_product_index].name, name);
        strcpy(prod[max_product_index].description, description);
        prod[max_product_index].cost = cost;
        prod[max_product_index].price = price;
    }
    else
    {
        printf("You must provide needed information for the new product\n");
    }
    product_count++;
    product_save(prod, product_csv);
}

void product_read(product *prod, float min_price, float max_price)
{
    int displayed = 0;
    {
        printf("==========================================================================================================================================\n");
        printf("| %-5s | %-20s | %-70s | %-10s | %-10s |\n",
               "ID", "Name", "Description", "Cost", "Price");
        printf("==========================================================================================================================================\n");

        for (int i = 1; i < product_count; i++)
        {
            if (prod[i].price >= min_price && prod[i].price <= max_price)
            {
                printf("| %-5d | %-20s | %-70s | %-10.2f | %-10.2f |\n",
                       prod[i].id,
                       prod[i].name,
                       prod[i].description,
                       prod[i].cost,
                       prod[i].price);
                displayed = 1;
            }
        }
        if (!displayed)
        {
            printf("| %-70s |\n", "No products found in the specified price range.");
        }
        printf("==========================================================================================================================================\n");
    }
}

// Function to update a specific product by ID
void product_update(product *prod, const char *product_csv, int id, char name[], char description[], float cost, float price)
{
    for (int i = 0; i < product_count; i++)
    {
        if (prod[i].id == id)
        {
            // Update fields only if valid arguments are provided
            if (strlen(name) > 0)
                strcpy(prod[i].name, name);
            if (strlen(description) > 0)
                strcpy(prod[i].description, description);
            if (cost >= 0)
                prod[i].cost = cost;
            if (price >= 0)
                prod[i].price = price;

            printf("Product with ID %d updated successfully.\n", id);
            return;
        }
    }
    printf("Product with ID %d not found.\n", id);
    product_save(prod, product_csv);
}

// Function to save the updated products back to the CSV file
void product_save(product *prod, const char *product_csv)
{
    FILE *shared_product = fopen(product_csv, "w");
    if (!shared_product)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < product_count; i++)
    {
        fprintf(shared_product, "%d,%s,%s,%.2f,%.2f\n",
                prod[i].id,
                prod[i].name,
                prod[i].description,
                prod[i].cost,
                prod[i].price);
    }

    fclose(shared_product);
    printf("Products saved successfully to %s.\n", product_csv);
}

void product_delete(product *prod, const char *product_csv, int id)
{
    int found = 0;

    for (int i = 0; i < product_count; i++)
    {
        if (prod[i].id == id)
        {
            found = 1;
            printf("Product with ID %d deleted successfully.\n", id);
            break;
        }
    }

    if (!found)
    {
        printf("Product with ID %d not found.\n", id);
        return;
    }

    // Save the updated product list back to the CSV without the deleted product
    FILE *file = fopen(product_csv, "w");
    if (!file)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < product_count; i++)
    {
        if (prod[i].id != id) // Skip the product to be deleted
        {
            fprintf(file, "%d,%s,%s,%.2f,%.2f\n",
                    prod[i].id,
                    prod[i].name,
                    prod[i].description,
                    prod[i].cost,
                    prod[i].price);
        }
    }

    fclose(file);
    product_load(prod, product_csv);
    printf("Updated product list saved successfully.\n");
}
