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

void inventory_add_product(inventory *inv, const char *inventory_csv, int id, const char *name, int stock, int threshold);

void inventory_delete_product(inventory *inv, const char *inventory_csv, int id);

void sync_inventory_with_product(const char *inventory_csv, const char *product_csv);

void inventory_adjust_threshold(inventory *inv, const char *inventory_csv, int id, int new_threshold);

void inventory_restock_from_file(inventory *inv, const char *inventory_csv, const char *restock_file);

extern int inventory_count;

extern int max_inventory_index;

extern inventory inv[];

extern char inventory_csv[];

#endif