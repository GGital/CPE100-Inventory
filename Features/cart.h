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

extern CartItem cart[];

extern float discount;

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

#endif