#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Authentication.h"

char authentication_csv[] = "./csv/authentication.csv";

unsigned long long int simple_hash(const char *password)
{
    unsigned long long int hash = 5381; // Starting value
    int c;

    while ((c = *password++))
    {
        hash = ((hash << 5) + hash) ^ c; // hash * 33 ^ c
    }

    return hash;
}

int loginUser(const char *authentication_csv, int privileges)
{
    char username[100], password[100];

    printf("\n======================================\n");
    printf("            User Login\n");
    printf("======================================\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    int result = validate_user(authentication_csv, username, password, privileges);

    if (result == 1)
    {
        if (privileges == 0)
        {
            printf("Welcome, Customer %s!\n", username);
        }
        else
        {
            printf("Welcome, Owner %s!\n", username);
        }
    }
    else
    {
        printf("Login failed. Please check your credentials and try again.\n");
    }
    return result;
}

// Function to validate the given username, password, and privileges
int validate_user(const char *authentication_csv, const char *username, const char *password, int privileges)
{
    // Hash the input password
    unsigned long long int hashed_password = simple_hash(password);

    // Open the CSV file in read mode
    FILE *file = fopen(authentication_csv, "r");
    if (!file)
    {
        printf("Error opening authentication.csv");
        return -1; // File open error
    }

    char line[256];
    char file_username[100];
    unsigned long long int file_hashed_password;
    int file_privileges;

    // Read each line of the CSV
    while (fgets(line, sizeof(line), file))
    {
        // Parse the line (CSV format: username,hashed_password,privileges)
        if (sscanf(line, "%[^,],%llu,%d", file_username, &file_hashed_password, &file_privileges) == 3)
        {
            // Check if the username, hashed password, and privileges match
            if (strcmp(username, file_username) == 0 &&
                hashed_password == file_hashed_password &&
                privileges == file_privileges)
            {
                fclose(file);
                printf("Validation successful for user: %s\n", username);
                return 1; // Successful validation
            }
        }
    }

    // Close the file
    fclose(file);

    printf("Validation failed for user: %s\n", username);
    return 0; // Validation failed
}

// Function to add a new user
int add_new_user(const char *authentication_csv, int privileges)
{
    char username[100], password[100];

    printf("\n======================================\n");
    printf("            User Creation\n");
    printf("======================================\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Validate privileges
    if (privileges != 0 && privileges != 1)
    {
        printf("Error: Privileges must be 0 (customer) or 1 (owner).\n");
        return -1;
    }

    // Hash the password
    unsigned long long int hashed_password = simple_hash(password);

    // Open the CSV file in append mode
    FILE *file = fopen(authentication_csv, "a");
    if (!file)
    {
        printf("Error opening authentication.csv");
        return -1;
    }

    // Write the user data to the file
    fprintf(file, "%s,%llu,%d\n", username, hashed_password, privileges);

    // Close the file
    fclose(file);

    printf("User added successfully: %s\n", username);
    return 0;
}