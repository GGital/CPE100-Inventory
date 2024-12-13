#include <stdio.h>
#include <string.h>
#include "Inventory.h"
#include "beautiful_cli.h"

int inventory_count = 0;     // Tracks the number of inventory items
int max_inventory_index = 0; // Tracks the maximum id in the inventory

char inventory_csv[] = "./csv/inventory.csv";

inventory inv[500];

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
                printf(ANSI_COLOR_YELLOW "%s with id %d is having low in quantity. Please restock.\n" ANSI_COLOR_RESET, inv[inventory_count - 1].name, inv[inventory_count - 1].id);
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
    // printf("Inventory saved successfully to %s.\n", inventory_csv);
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

void inventory_restock_from_file(inventory *inv, const char *inventory_csv, const char *restock_file)
{
    FILE *file = fopen(restock_file, "r");
    if (!file)
    {
        printf("Error: Unable to open restock file: %s\n", restock_file);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        int product_id, quantity;
        if (sscanf(line, "%d : %d", &product_id, &quantity) == 2)
        {
            // Call existing inventory_restock function to update stock
            inventory_restock(inv, inventory_csv, product_id, quantity);
        }
        else
        {
            printf("Warning: Invalid line format in restock file: %s", line);
        }
    }

    fclose(file);
    printf(ANSI_COLOR_GREEN "Restock operation completed successfully.\n" ANSI_COLOR_RESET);
}

void inventory_add_product(inventory *inv, const char *inventory_csv, int id, const char *name, int stock, int threshold)
{
    // Check if the inventory is full
    if (inventory_count >= 500)
    {
        printf("Error: Inventory is full. Cannot add more products.\n");
        return;
    }

    // Check if the product ID already exists
    for (int i = 0; i < inventory_count; i++)
    {
        if (inv[i].id == id)
        {
            printf("Error: Product with ID %d already exists.\n", id);
            return;
        }
    }

    // Add the new product to the inventory array
    inv[inventory_count].id = id;
    strcpy(inv[inventory_count].name, name);
    inv[inventory_count].stock = stock;
    inv[inventory_count].threshold = threshold;

    inventory_count++;
    printf("Product added successfully: %s (ID: %d)\n", name, id);

    // Save the updated inventory to the CSV file
    inventory_save(inv, inventory_csv);
}

void inventory_delete_product(inventory *inv, const char *inventory_csv, int id)
{
    int found = 0; // Flag to track if the product is found

    // Iterate through the inventory to find the product
    for (int i = 0; i < inventory_count; i++)
    {
        if (inv[i].id == id)
        {
            found = 1;

            // Shift the remaining elements in the array to fill the gap
            for (int j = i; j < inventory_count - 1; j++)
            {
                inv[j] = inv[j + 1];
            }

            inventory_count--; // Decrease the inventory count
            printf("Product with ID %d deleted successfully.\n", id);
            break;
        }
    }

    // If the product was not found, print an error message
    if (!found)
    {
        printf("Error: Product with ID %d not found.\n", id);
        return;
    }

    // Save the updated inventory to the CSV file
    inventory_save(inv, inventory_csv);
}

void sync_inventory_with_product(const char *inventory_csv, const char *product_csv)
{
    FILE *product_file = fopen(product_csv, "r");
    if (!product_file)
    {
        printf("Error: Unable to open product.csv.\n");
        return;
    }

    inventory new_inventory[500];
    int new_inventory_count = 0;

    char line[500];
    while (fgets(line, sizeof(line), product_file))
    {
        if (new_inventory_count == 0)
        {
            // Skip the header line
            new_inventory_count++;
            continue;
        }

        int id;
        char name[100];
        int stock = 0;     // Default stock
        int threshold = 0; // Default threshold

        sscanf(line, "%d,%99[^,]", &id, name);

        // Check if the product already exists in the current inventory
        int found = 0;
        for (int i = 0; i < inventory_count; i++)
        {
            if (inv[i].id == id)
            {
                found = 1;
                new_inventory[new_inventory_count - 1] = inv[i];
                break;
            }
        }

        // If not found, add it as a new product with default values
        if (!found)
        {
            new_inventory[new_inventory_count - 1].id = id;
            strcpy(new_inventory[new_inventory_count - 1].name, name);
            new_inventory[new_inventory_count - 1].stock = stock;
            new_inventory[new_inventory_count - 1].threshold = threshold;
        }

        new_inventory_count++;
    }
    fclose(product_file);

    // Update the inventory array and count
    memcpy(inv, new_inventory, sizeof(new_inventory));
    inventory_count = new_inventory_count - 1;

    // Save the updated inventory to inventory.csv
    inventory_save(inv, inventory_csv);

    // printf("Synchronization complete.\n");
}

void inventory_adjust_threshold(inventory *inv, const char *inventory_csv, int id, int new_threshold)
{
    int found = 0; // Flag to track if the product is found

    // Iterate through the inventory to find the product
    for (int i = 0; i < inventory_count; i++)
    {
        if (inv[i].id == id)
        {
            inv[i].threshold = new_threshold; // Update the threshold
            printf("Threshold for product ID %d updated to %d.\n", id, new_threshold);
            found = 1;
            break;
        }
    }

    // If the product was not found, print an error message
    if (!found)
    {
        printf("Error: Product with ID %d not found.\n", id);
        return;
    }

    // Save the updated inventory to the CSV file
    inventory_save(inv, inventory_csv);
}