#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Authentication.h"
#include "owner.h"
#include "CRUD.h"
#include "Inventory.h"
#include "cart.h"
#include "coupon.h"
#include "beautiful_cli.h"
#include "customer.h"
#ifdef _WIN32
#include <windows.h>
#endif

void enable_ansi_colors()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
}

// Compilenation code
// gcc -mconsole -std=gnu11 -o program -I./Sub-Interface -I./Features main.c Sub-Interface/beautiful_cli.c Sub-Interface/owner.c Sub-Interface/customer.c Features/coupon.c Features/cart.c Features/CRUD.c Features/Inventory.c Features/Authentication.c Features/report.c Features/logging.c

int result;

int main()
{
    enable_ansi_colors();
    inventory_load(inv, inventory_csv);
    product_load(prod, product_csv);
    coupon_load(cou, coupon_csv);
    Load_cart(cart_csv);
    Process_Scheduled_Purchases(scheduled_csv);

    sync_inventory_with_product(inventory_csv, product_csv);

    delete_expired_coupons(cou);

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
        clearScreen();
        // Handle user input
        switch (choice)
        {
        case 1:
            add_new_user(authentication_csv, 0);
            break;
        case 2:
            result = loginUser(authentication_csv, 0);
            if (result)
                customerPrivilegesMenu();
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