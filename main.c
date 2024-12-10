#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Authentication.h"
#include "owner.h"
#include "CRUD.h"
#include "Inventory.h"
#include "coupon.h"

// Compilenation code
// gcc -o program -I./Sub-Interface -I./Features main.c Sub-Interface/owner.c Features/coupon.c Features/CRUD.c Features/Inventory.c Features/Authentication.c

const char product_csv[] = "./csv/product.csv";
const char inventory_csv[] = "./csv/inventory.csv";
const char authentication_csv[] = "./csv/authentication.csv";

int result;

int main()
{

    product prod[500];
    inventory inv[500];

    inventory_load(inv, inventory_csv);
    product_load(prod, product_csv);

    int choice;

    do
    {
        // Display the menu
        printf("\n======================================\n");
        printf("      Inventory System Management\n");
        printf("======================================\n");
        printf("1. Create Customer User\n");
        printf("2. Login Customer User\n");
        printf("3. Login Owner User\n");
        printf("0. Exit\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handle user input
        switch (choice)
        {
        case 1:
            add_new_user(authentication_csv, 0);
            break;
        case 2:
            result = loginUser(authentication_csv, 0);
            break;
        case 3:
            result = loginUser(authentication_csv, 1);
            if (result)
                ownerPrivilegesMenu();
            break;
        case 0:
            printf("\nExiting... Thank you for using the system!\n");
            break;
        default:
            printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}