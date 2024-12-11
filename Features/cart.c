#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cart.h"
#include "Inventory.h"
#include "beautiful_cli.h"
#include "CRUD.h"
#include "coupon.h"

#define MAX_CART_ITEMS 500

char cart_csv[] = "./csv/cart.csv";

CartItem cart[500];
int cart_size = 0;
float total_cost = 0.0;
float discount = 0.0;
char used_coupon_name[200] = "";

void Save_cart(const char *cart_csv)
{
    FILE *file = fopen(cart_csv, "w");
    if (!file)
    {
        printf("Error: Unable to open file %s for writing.\n", cart_csv);
        return;
    }

    fprintf(file, "username,product_id,quantity,price_per_unit,total_price\n");
    for (int i = 0; i < cart_size; i++)
    {
        fprintf(file, "%s,%d,%d,%.2f,%.2f\n", cart[i].username, cart[i].product_id, cart[i].quantity, cart[i].price_per_unit, cart[i].total_price);
    }

    fclose(file);
}

void Load_cart(const char *cart_csv)
{
    cart_size = 0;
    FILE *file = fopen(cart_csv, "r");
    if (!file)
    {
        printf("Error: Unable to open file %s for reading.\n", cart_csv);
        return;
    }

    char line[500];
    while (fgets(line, sizeof(line), file))
    {
        if (cart_size == 0)
        {
            cart_size++; // Skip the header
            continue;
        }

        int col = 0;
        char *value = strtok(line, ",");
        while (value)
        {
            if (col == 0)
                strcpy(cart[cart_size - 1].username, value);
            else if (col == 1)
                sscanf(value, "%d", &cart[cart_size - 1].product_id);
            else if (col == 2)
                sscanf(value, "%d", &cart[cart_size - 1].quantity);
            else if (col == 3)
                sscanf(value, "%lf", &cart[cart_size - 1].price_per_unit);
            else if (col == 4)
                sscanf(value, "%lf", &cart[cart_size - 1].total_price);

            value = strtok(NULL, ",");
            col++;
        }
        cart_size++;
    }
    fclose(file);
    cart_size--;
}

void Add_product_to_cart(const char *username, int product_id, int quantity, double price_per_unit)
{
    for (int i = 0; i < cart_size; i++)
    {
        if (strcmp(cart[i].username, username) == 0 && cart[i].product_id == product_id)
        {
            cart[i].quantity += quantity;
            cart[i].total_price = cart[i].quantity * cart[i].price_per_unit;
            printf("Updated product %d for user %s: Quantity = %d\n", product_id, username, cart[i].quantity);
            Save_cart(cart_csv);
            return;
        }
    }

    if (cart_size >= MAX_CART_ITEMS)
    {
        printf("Cart is full. Cannot add more items.\n");
        return;
    }
    printf("%d\n", cart_size);
    strcpy(cart[cart_size].username, username);
    cart[cart_size].product_id = product_id;
    cart[cart_size].quantity = quantity;
    cart[cart_size].price_per_unit = price_per_unit;
    cart[cart_size].total_price = quantity * price_per_unit;
    cart_size++;
    printf("%d\n", cart_size);
    printf("Added product %d for user %s: Quantity = %d\n", product_id, username, quantity);
    Save_cart(cart_csv);
}

void Remove_product_from_cart(const char *username, int product_id)
{
    for (int i = 0; i < cart_size; i++)
    {
        if (strcmp(cart[i].username, username) == 0 && cart[i].product_id == product_id)
        {
            for (int j = i; j < cart_size - 1; j++)
            {
                cart[j] = cart[j + 1];
            }
            cart_size--;
            printf("Removed product %d for user %s\n", product_id, username);
            Save_cart(cart_csv);
            return;
        }
    }
    printf("Product %d not found for user %s\n", product_id, username);
}

void View_Cart(const char *username)
{
    int found = 0;
    double all_total_price = 0;
    char product_name[100];
    printf("==========================================================================\n");
    printf("| %-10s | %-20s | %-10s | %-10s | %-10s | %-10s |\n", "Product ID", "Product Name", "Quantity", "Price/Unit", "Total", "Username");
    printf("==========================================================================\n");

    for (int i = 0; i < cart_size; i++)
    {
        if (strcmp(cart[i].username, username) == 0)
        {
            for (int j = 0; j < product_count; j++)
            {
                if (cart[i].product_id == prod[j].id)
                {
                    strcpy(product_name, prod[j].name);
                }
            }
            printf("| %-10d | %-20s | %-10d | %-10.2f | %-10.2f | %-10s |\n",
                   cart[i].product_id, product_name, cart[i].quantity, cart[i].price_per_unit, cart[i].total_price, cart[i].username);
            found = 1;
            all_total_price += cart[i].total_price;
        }
    }
    printf("==========================================================================\n");
    printf(ANSI_COLOR_GREEN "Total price for this cart is %.2lf\n" ANSI_COLOR_RESET, all_total_price);

    if (strlen(used_coupon_name) > 0)
    {
        printf(ANSI_COLOR_BLUE "A coupon '%s' is applied.\n" ANSI_COLOR_RESET, used_coupon_name);
        printf(ANSI_COLOR_BLUE "Discount applied: %.2f\n" ANSI_COLOR_RESET, discount);
        printf(ANSI_COLOR_BLUE "Price after discount: %.2f\n" ANSI_COLOR_RESET, all_total_price - discount);
    }
    else if (!found)
    {
        printf("| %-70s |\n", "Your cart is empty.");
    }
    else
    {
        printf(ANSI_COLOR_YELLOW "No coupon is applied to this cart.\n" ANSI_COLOR_RESET);
    }
    printf("==========================================================================\n");
}

int check_for_unavailable(int product_id, int i, int inventory_index)
{

    if (inventory_index == -1)
    {
        printf("Error: Product ID %d not found in inventory. Removing from cart.\n", product_id);
        for (int j = i; j < cart_size - 1; j++)
        {
            cart[j] = cart[j + 1];
        }
        cart_size--;
        return 0;
    }

    // Check stock
    if (cart[i].quantity > inv[inventory_index].stock)
    {
        printf("Insufficient stock for product '%s'. Removing from cart.\n", inv[inventory_index].name);
        for (int j = i; j < cart_size - 1; j++)
        {
            cart[j] = cart[j + 1];
        }
        cart_size--;
        return 0;
    }
    return 1;
}

void Checkout(const char *username)
{
    double total_checkout_price = 0.0;
    int found = 0;
    printf("Checking out for %s:\n", username);
    printf("--------------------------------\n");

    for (int i = 0; i < cart_size;)
    {
        if (strcmp(cart[i].username, username) == 0)
        {
            int inventory_index = -1;
            int product_id = cart[i].product_id;

            // Find product in inventory
            for (int j = 0; j < inventory_count; j++)
            {
                if (inv[j].id == product_id)
                {
                    inventory_index = j;
                    break;
                }
            }

            int res = check_for_unavailable(cart[i].product_id, i, inventory_index);

            // Deduct stock
            inv[inventory_index].stock -= cart[i].quantity;
            printf("Product '%s': Quantity %d, Price/Unit: %.2f, Total: %.2f\n",
                   inv[inventory_index].name, cart[i].quantity,
                   cart[i].price_per_unit, cart[i].total_price);

            total_checkout_price += cart[i].total_price;

            // Remove product from cart
            for (int j = i; j < cart_size - 1; j++)
            {
                cart[j] = cart[j + 1];
            }
            cart_size--;
            found = 1;
        }
        else
        {
            i++;
        }
    }

    // Apply active coupon if any
    if (found)
    {
        if (strlen(used_coupon_name) > 0)
        {
            printf("Used coupon: %s\n", used_coupon_name);

            // Recalculate discount for the total checkout price
            for (int i = 0; i < coupon_count; i++)
            {
                if (strcmp(cou[i].name, used_coupon_name) == 0)
                {
                    if (total_checkout_price >= cou[i].minimum_cost)
                    {
                        discount = (cou[i].discountRate / 100.0) * total_checkout_price;
                        printf("You saved: %.2f\n", discount);
                        total_checkout_price -= discount;
                        strcpy(used_coupon_name, "");
                    }
                    else
                    {
                        printf("Note: Your cart total no longer meets the minimum requirement for the coupon '%s'.\n", used_coupon_name);
                        discount = 0.0;
                    }
                    break;
                }
            }
        }

        printf("Total checkout price after discounts: %.2f\n", total_checkout_price);
        printf("Checkout complete. Your cart has been cleared.\n");
        Save_cart(cart_csv);
    }
    else
    {
        printf("Your cart is empty or no products were eligible for checkout.\n");
    }
    printf("--------------------------------\n");
}

void Apply_coupon(const char *username, const char *coupon_name)
{
    total_cost = 0.0;
    for (int i = 0; i < cart_size; i++)
    {
        if (strcmp(cart[i].username, username) == 0)
        {
            total_cost += cart[i].total_price;
        }
    }

    for (int i = 0; i < coupon_count; i++)
    {
        if (strcmp(cou[i].name, coupon_name) == 0)
        {
            if (total_cost >= cou[i].minimum_cost)
            {
                discount = (cou[i].discountRate / 100.0) * total_cost;
                printf("Coupon '%s' applied! You saved %.2f\n", coupon_name, discount);
                total_cost -= discount;
                printf("New total cost: %.2f\n", total_cost);
                strcpy(used_coupon_name, coupon_name);
            }
            else
            {
                printf("Coupon '%s' requires a minimum cart total of %.2f\n", coupon_name, cou[i].minimum_cost);
            }
            return;
        }
    }
    printf("Coupon '%s' not found.\n", coupon_name);
}

void Deactivate_coupon(const char *coupon_name)
{
    for (int i = 0; i < coupon_count; i++)
    {
        if (strcmp(cou[i].name, coupon_name) == 0)
        {
            printf("Coupon '%s' deactivated. Reverting prices.\n", coupon_name);
            total_cost += discount;
            discount = 0.0;
            printf("Total cost reverted to: %.2f\n", total_cost);
            strcpy(used_coupon_name, "");
            return;
        }
    }
    printf("Coupon '%s' not found or already deactivated.\n", coupon_name);
}