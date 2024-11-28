#include <stdio.h>
#include "feature1.h"
#include "CRUD.h"
#include "Inventory.h"

// Compilenation code
// gcc -I./Features main.c Features/CRUD.c Features/Inventory.c Features/feature1.c -o program

const char product_csv[] = "./csv/product.csv";
const char inventory_csv[] = "./csv/inventory.csv";

int main()
{

    product prod[500];
    inventory inv[500];

    inventory_load(inv, inventory_csv);
    product_load(prod, product_csv);

    product_read(prod, 0, 10000);

    // Update a product (Example: ID = 1)
    product_update(prod, product_csv, 1, "New Name", "", -1, 99.99);

    product_read(prod, 0, 10000);

    product_delete(prod, product_csv, 2);

    product_read(prod, 50, 80);

    // printf("%d", product_count);

    inventory_check_stock(inv);

    inventory_restock(inv, inventory_csv, 1, 5);

    inventory_check_stock(inv);

    return 0;
}