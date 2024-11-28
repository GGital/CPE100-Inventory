#include <stdio.h>
#include <string.h>
#include "Inventory.h"

int inventory_count = 0;     // Tracks the number of inventory items
int max_inventory_index = 0; // Tracks the maximum id in the inventory

void inventory_load(inventory *inv, const char *inventory_csv)
{
    inventory_count = 0;                    // Initialize inventory count
    FILE *file = fopen(inventory_csv, "r"); // Open the CSV file
    if (!file)
    {
        printf("Failed to open file: %s\n", inventory_csv);
        return;
    }

    char line[500];
    while (fgets(line, sizeof(line), file)) // Read each line of the file
    {
        if (inventory_count == 0)
        {
            // Skip the header line
            inventory_count++;
            continue;
        }

        int col = 0;
        char *value = strtok(line, ","); // Split the line into tokens
        while (value)
        {
            if (col == 0)
                sscanf(value, "%d", &inv[inventory_count - 1].id);
            else if (col == 1)
                strcpy(inv[inventory_count - 1].name, value);
            else if (col == 2)
                sscanf(value, "%d", &inv[inventory_count - 1].stock);
            else if (col == 3)
                sscanf(value, "%d", &inv[inventory_count - 1].threshold);

            if (inv[inventory_count - 1].stock < inv[inventory_count - 1].threshold)
            {
                printf("%s with id %d is having low in quantity. Please restock.\n", inv[inventory_count - 1].name, inv[inventory_count - 1].id);
            }
            value = strtok(NULL, ",");
            col++;
        }
        max_inventory_index = (max_inventory_index < inv[inventory_count - 1].id)
                                  ? inv[inventory_count - 1].id
                                  : max_inventory_index;
        inventory_count++;
    }
    fclose(file);
}

// Function to save the updated inventory back to the CSV file
void inventory_save(inventory *inv, const char *inventory_csv)
{
    FILE *file = fopen(inventory_csv, "w");
    if (!file)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    // Write the header row
    fprintf(file, "id,name,quantity_in_stock,threshold_quantity\n");

    // Write each inventory item
    for (int i = 0; i < inventory_count; i++)
    {
        fprintf(file, "%d,%s,%d,%d\n",
                inv[i].id,
                inv[i].name,
                inv[i].stock,
                inv[i].threshold);
    }

    fclose(file);
    printf("Inventory saved successfully to %s.\n", inventory_csv);
}

void inventory_check_stock(inventory *inv)
{
    int displayed = 0;

    printf("======================================================================\n");
    printf("| %-5s | %-20s | %-15s | %-10s | %-15s |\n",
           "ID", "Name", "Stock", "Threshold", "Notification");
    printf("======================================================================\n");

    for (int i = 0; i < inventory_count; i++)
    {
        char notification[20] = ""; // Notification field
        if (inv[i].stock < inv[i].threshold)
        {
            strcpy(notification, "LOW STOCK!"); // Add "LOW STOCK!" notification
        }

        printf("| %-5d | %-20s | %-15d | %-10d | %-15s |\n",
               inv[i].id,
               inv[i].name,
               inv[i].stock,
               inv[i].threshold,
               notification);
        displayed = 1;
    }

    if (!displayed)
    {
        printf("| %-70s |\n", "No inventory data found.");
    }
    printf("======================================================================\n");
}

void inventory_restock(inventory *inv, const char *inventory_csv, int id, int quantity)
{
    int found = 0; // Flag to check if the product exists

    // Iterate through the inventory to find the product with the given ID
    for (int i = 0; i < inventory_count; i++)
    {
        if (inv[i].id == id)
        {
            inv[i].stock += quantity; // Increase the stock by the given quantity
            printf("Restocked %s. New stock: %d\n", inv[i].name, inv[i].stock);
            found = 1; // Mark as found
            break;
        }
    }

    // If the product was not found, print an error message
    if (!found)
    {
        printf("Error: Product with ID %d not found.\n", id);
        return;
    }

    // Save the updated inventory back to the CSV file
    inventory_save(inv, inventory_csv);
}
