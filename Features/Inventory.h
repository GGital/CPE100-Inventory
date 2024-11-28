#ifndef Inventory
#define Inventory

typedef struct
{
    int id;
    char name[150];
    int stock;
    int threshold;
} inventory;

void inventory_load(inventory *inv, const char *inventory_csv);

void inventory_save(inventory *inv, const char *inventory_csv);

void inventory_check_stock(inventory *inv);

void inventory_restock(inventory *inv, const char *inventory_csv, int id, int quantity);

extern int inventory_count;

extern int max_inventory_index;

#endif