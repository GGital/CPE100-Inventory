#ifndef CRUD
#define CRUD

typedef struct
{
    int id;
    char name[150];
    char description[250];
    float cost;
    float price;
} product;

void product_load(product *prod, const char *product_csv);

void product_read(product *prod, float min_price, float max_price);

void product_create(product *prod, const char *product_csv, char name[], char description[], float cost, float price);

void product_update(product *prod, const char *product_csv, int id, char name[], char description[], float cost, float price);

void product_save(product *prod, const char *product_csv);

void product_delete(product *prod, const char *product_csv, int id);

extern int product_count;

extern int max_product_index;

extern product prod[];

extern char product_csv[];

#endif