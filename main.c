#include <stdio.h>
#include "feature1.h"
#include "CRUD.h"

const char product_csv[] = "./csv/product.csv";

int main()
{

    product prod[500];

    product_load(prod, product_csv);

    product_read(prod, 0, 10000);

    // Update a product (Example: ID = 1)
    product_update(prod, product_csv, 1, "New Name", "", -1, 99.99);

    product_read(prod, 0, 10000);

    product_delete(prod, product_csv, 2);

    product_read(prod, 50, 80);

    printf("%d", product_count);
    return 0;
}