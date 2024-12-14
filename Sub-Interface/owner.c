#include <string.h>
#include <stdlib.h>
#include "owner.h"
#include "CRUD.h"
#include "coupon.h"
#include "Inventory.h"
#include "beautiful_cli.h"
#include "report.h"
#include "cart.h"
#include "logging.h"
#include "Authentication.h"

void ownerPrivilegesMenu()
{
    int choice;

    do
    {
        // Display the main menu
        printf("\n======================================\n");
        printf("         Owner Privileges Menu\n");
        printf("======================================\n");
        printf("1. Coupon Management\n");
        printf("2. CRUD Operations\n");
        printf("3. Inventory Management\n");
        printf("4. Report Creation\n");
        printf("5. Edit Price Filter\n");
        printf("0. Exit\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();

        // Handle user input
        switch (choice)
        {
        case 1:
            couponMenu();
            break;
        case 2:
            crudMenu();
            break;
        case 3:
            inventoryMenu();
            break;
        case 4:
            char date_filter[100];
            printf("\n======================================\n");
            printf("            Report Creation\n");
            printf("======================================\n");
            printf("Enter date of the report (YYYY-MM-DD): ");
            scanf("%s", date_filter);
            Generate_report(transaction_txt, date_filter, min_price_filter, max_price_filter, "", prod, product_count);
            break;
        case 5:
            printf("\n======================================\n");
            printf("            Price Filtering\n");
            printf("======================================\n");
            printf("Enter minimum price for filter: ");
            scanf("%f", &min_price_filter);
            printf("Enter maximum price for filter: ");
            scanf("%f", &max_price_filter);
            break;
        case 0:
            printf("\nExiting Owner Privileges Menu...\n");
            break;
        default:
            printf("\nInvalid choice! Please try again.\n");
            break;
        }
    } while (choice != 0);
}

// Coupon menu function
void couponMenu()
{
    char buffer[100];
    int choice;
    do
    {
        printf("\n======================================\n");
        printf("            Coupon Management\n");
        printf("======================================\n");
        printf("1. Add Coupon\n");
        printf("2. View Coupons\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();

        switch (choice)
        {
        case 1:
            char name[500];
            float minimum_cost, discountRate;
            int day, month, year;
            printf("\n======================================\n");
            printf("            Coupon Creation\n");
            printf("======================================\n");
            printf("Enter name of coupon: ");
            scanf("%s", name);
            printf("Enter minimum_cost: ");
            scanf("%f", &minimum_cost);
            printf("Enter discount rate for the coupon: ");
            scanf("%f", &discountRate);
            printf("When will this coupon expire (dd/mm/yyyy) : ");
            scanf("%d/%d/%d", &day, &month, &year);
            animated_spinner(10);
            strcpy(buffer, "Add coupon : ");
            strcat(buffer, name);
            log_user_action(put_username, buffer);
            coupon_create(cou, coupon_csv, name, minimum_cost, discountRate, day, month, year);
            break;
        case 2:
            float minimum_discount, maximum_discount;
            printf("\n======================================\n");
            printf(ANSI_COLOR_GREEN "            List of Coupon\n" ANSI_COLOR_RESET);
            printf("======================================\n");
            printf("Enter minimum discount rate: ");
            scanf("%f", &minimum_discount);
            printf("Enter maximum discount rate: ");
            scanf("%f", &maximum_discount);
            coupon_read(cou, minimum_discount, maximum_discount);
            break;
        case 0:
            printf("Returning to Owner Privileges Menu...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
            break;
        }
    } while (choice != 0);
}

// CRUD operations menu function
void crudMenu()
{
    int choice;
    char name[500], description[500];
    char buffer[100];
    char buffer2[33];
    float cost, price;
    int id;
    do
    {
        printf("\n======================================\n");
        printf("            CRUD Operations\n");
        printf("======================================\n");
        printf("1. Create Item\n");
        printf("2. Read Items\n");
        printf("3. Update Item\n");
        printf("4. Delete Item\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();

        switch (choice)
        {
        case 1:
            printf("\n======================================\n");
            printf("            Inserting New Product\n");
            printf("======================================\n");
            printf("Enter name of product: ");
            scanf("%s", name);
            printf("Enter description of product: ");
            scanf("%s", description);
            printf("Enter cost (principal): ");
            scanf("%f", &cost);
            printf("Enter selling price: ");
            scanf("%f", &price);
            animated_spinner(10);
            strcpy(buffer, "Insert new product name :");
            strcat(buffer, name);
            log_user_action(put_username, buffer);
            product_create(prod, product_csv, name, description, cost, price);
            break;
        case 2:
            product_read(prod, min_price_filter, max_price_filter);
            break;
        case 3:
            printf("\n======================================\n");
            printf("            Updating Product\n");
            printf("======================================\n");
            printf("Enter id of product: ");
            scanf("%d", &id);
            printf("Enter name of product: ");
            scanf("%s", name);
            printf("Enter description of product: ");
            scanf("%s", description);
            printf("Enter cost (principal): ");
            scanf("%f", &cost);
            printf("Enter selling price: ");
            scanf("%f", &price);
            animated_spinner(10);
            strcpy(buffer, "Update product name :");
            strcat(buffer, name);
            log_user_action(put_username, buffer);
            product_update(prod, product_csv, id, name, description, cost, price);
            break;
        case 4:
            printf("\n======================================\n");
            printf("            Deleting Product\n");
            printf("======================================\n");
            printf("Enter id of product: ");
            scanf("%d", &id);
            animated_spinner(10);
            strcpy(buffer, "Delete Product ID :");
            itoa(id, buffer2, 10);
            strcat(buffer, buffer2);
            log_user_action(put_username, buffer);
            product_delete(prod, product_csv, id);
            break;
        case 0:
            printf("Returning to Owner Privileges Menu...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);
}

// Inventory management menu function
void inventoryMenu()
{
    char buffer[100];
    char buffer2[33];
    int choice, id, quantity;
    do
    {
        printf("\n======================================\n");
        printf("            Inventory Management\n");
        printf("======================================\n");
        printf("1. View Inventory\n");
        printf("2. Add Stock\n");
        printf("3. Restock From file\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();
        switch (choice)
        {
        case 1:
            inventory_check_stock(inv);
            break;
        case 2:
            printf("\n======================================\n");
            printf("            Inventory Restocking\n");
            printf("======================================\n");
            printf("Enter id of product: ");
            scanf("%d", &id);
            printf("Enter quantity of product to be restocked: ");
            scanf("%d", &quantity);
            strcpy(buffer, "Restocking Product ID :");
            itoa(id, buffer2, 10);
            strcat(buffer, buffer2);
            log_user_action(put_username, buffer);
            inventory_restock(inv, inventory_csv, id, quantity);
            break;
        case 3:
            printf("\n======================================\n");
            printf("            Restock From File\n");
            printf("======================================\n");
            printf("Enter name of file (File should be inside stock_script): ");
            scanf("%s", buffer2);
            strcat(buffer2, ".txt");
            strcpy(buffer, "./stock_script/");
            strcat(buffer, buffer2);
            inventory_restock_from_file(inv, inventory_csv, buffer);
            break;
        case 0:
            printf("Returning to Owner Privileges Menu...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);
}