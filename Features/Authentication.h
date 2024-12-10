#ifndef Authentication
#define Authentication

unsigned long long int simple_hash(const char *password);

int validate_user(const char *authentication_csv, const char *username, const char *password, int privileges);

int loginUser(const char *authentication_csv, int privileges);

int add_new_user(const char *authentication_csv, int privileges);

#endif