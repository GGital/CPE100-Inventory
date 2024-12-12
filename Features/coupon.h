#ifndef Coupon
#define Coupon

typedef struct
{
    int id;
    char name[150];
    float minimum_cost;
    float discountRate;
    int expiry_day;
    int expiry_month;
    int expiry_year;
} coupon;

void coupon_load(coupon *coupons, const char *coupon_csv);

void coupon_read(coupon *coupons, float min_discount, float max_discount);

void coupon_save(coupon *coupons, const char *coupon_csv);

void delete_expired_coupons(coupon *coupons);

void coupon_create(coupon *coupons, const char *coupon_csv, char name[], float minimum_cost, float discountRate, int expiry_day, int expiry_month, int expiry_year);

void delete_coupon_by_name(coupon *coupons, const char *coupon_csv, const char *name_to_delete);

extern int coupon_count;

extern int max_coupon_index;

extern coupon cou[];

extern char coupon_csv[];

#endif