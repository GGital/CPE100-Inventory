#include "owner.h"
#include "CRUD.h"
#include "coupon.h"
#include "Inventory.h"

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
        printf("0. Exit\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

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
        case 0:
            printf("\nExiting Owner Privileges Menu...\n");
            break;
        default:
            printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 0);
}

// Coupon menu function
void couponMenu()
{
    int choice;
    do
    {
        printf("\n--- Coupon Management ---\n");
        printf("1. Add Coupon\n");
        printf("2. View Coupons\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

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
            coupon_create(cou, coupon_csv, name, minimum_cost, discountRate, day, month, year);
            break;
        case 2:
            float minimum_discount, maximum_discount;
            printf("\n======================================\n");
            printf("            List of Coupon\n");
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
        }
    } while (choice != 0);
}

// CRUD operations menu function
void crudMenu()
{
    int choice;
    do
    {
        printf("\n--- CRUD Operations ---\n");
        printf("1. Create Item\n");
        printf("2. Read Items\n");
        printf("3. Update Item\n");
        printf("4. Delete Item\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Feature to create item coming soon...\n");
            break;
        case 2:
            printf("Feature to read items coming soon...\n");
            break;
        case 3:
            printf("Feature to update item coming soon...\n");
            break;
        case 4:
            printf("Feature to delete item coming soon...\n");
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
    int choice;
    do
    {
        printf("\n--- Inventory Management ---\n");
        printf("1. View Inventory\n");
        printf("2. Add Stock\n");
        printf("3. Remove Stock\n");
        printf("0. Back to Owner Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Feature to view inventory coming soon...\n");
            break;
        case 2:
            printf("Feature to add stock coming soon...\n");
            break;
        case 3:
            printf("Feature to remove stock coming soon...\n");
            break;
        case 0:
            printf("Returning to Owner Privileges Menu...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);
}