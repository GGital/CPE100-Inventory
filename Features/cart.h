#ifndef CART
#define CART

typedef struct CartItem
{
    char username[50];
    int product_id;
    int quantity;
    double price_per_unit;
    double total_price;
} CartItem;

typedef struct
{
    char name_of_txt[100];
    char datetime_str[100];
} ScheduledPurchaseEntry;

extern CartItem cart[];

extern char transaction_txt[];

extern char used_coupon_name[];

extern float discount;

extern char scheduled_csv[];

extern float total_cost;

extern char cart_csv[];

extern int cart_size;

void Load_cart(const char *cart_csv);

void Save_cart(const char *cart_csv);

void Add_product_to_cart(const char *username, int product_id, int quantity, double price_per_unit);

void Remove_product_from_cart(const char *username, int product_id);

void View_Cart(const char *username);

void Checkout(const char *username);

void Apply_coupon(const char *username, const char *coupon_name);

void Deactivate_coupon(const char *coupon_name);

void Process_Scheduled_Purchases(const char *csv_path);

#endif