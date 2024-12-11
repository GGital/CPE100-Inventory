#include "CRUD.h"
#include "coupon.h"
#include "Inventory.h"
#include "beautiful_cli.h"
#include "customer.h"
#include "Authentication.h"
#include "cart.h"

void customerPrivilegesMenu()
{
    int choice;
    int id, quantity;
    double price_per_unit;
    do
    {
        // Display the main menu
        printf("\n======================================\n");
        printf("        Customer Privileges Menu\n");
        printf("======================================\n");
        printf("1. View Product List\n");
        printf("2. Add Product to Cart\n");
        printf("3. View Cart\n");
        printf("4. Checkout\n");
        printf("5. Apply Discount Coupon\n");
        printf("0. Exit\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handle user input
        switch (choice)
        {
        case 1:
            product_read(prod, 0, 99999);
            break;
        case 2:
            printf("\n======================================\n");
            printf(ANSI_COLOR_YELLOW "            Adding Product to Cart\n" ANSI_COLOR_RESET);
            printf("======================================\n");
            printf("Enter id of product: ");
            scanf("%d", &id);
            printf("Enter quantity of product to purchase: ");
            scanf("%d", &quantity);
            for (int i = 0; i < product_count; i++)
            {
                if (id == prod[i].id)
                {
                    price_per_unit = prod[i].price;
                    break;
                }
            }
            animated_spinner(10);
            Add_product_to_cart(put_username, id, quantity, price_per_unit);
            break;
        case 3:
            View_Cart(put_username);
            break;
        case 4:
            Checkout(put_username);
            break;
        case 5:
            char coupon_name[100];
            printf("\n======================================\n");
            printf(ANSI_COLOR_YELLOW "            Applying Discount Coupon\n" ANSI_COLOR_RESET);
            printf("======================================\n");
            printf("Enter name of coupon: ");
            scanf("%s", coupon_name);
            Apply_coupon(put_username, coupon_name);
            break;
        case 0:
            printf("\nExiting Customer Privileges Menu...\n");
            break;
        default:
            printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 0);
}