#ifndef Report
#define Report

#include "CRUD.h"

typedef struct
{
    int product_id;
    int quantity;
    double price;
} SoldProduct;

typedef struct
{
    int product_id;
    int total_quantity;
} ProductSummary;

void Parse_transaction(const char *transaction_txt, const char *date_filter, SoldProduct sold_products[], int *product_count, double *total_revenue_after_discount);

void Generate_report(const char *transaction_txt, const char *date_filter, double min_price, double max_price, const char *product_name_filter, product prod[], int total_products);

#endif