#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char log_txt[] = "./txt/log.txt";

int log_user_action(const char *username, const char *action_type)
{
    // Validate input parameters
    if (username == NULL || action_type == NULL)
    {
        printf("Error: Invalid input parameters\n");
    }

    // Open file in append mode
    FILE *log_file = fopen(log_txt, "a");
    if (log_file == NULL)
    {
        printf("Error: Could not open log file\n");
    }

    // Get current time
    time_t current_time;
    time(&current_time);

    // Convert time to local time string
    char *time_str = ctime(&current_time);

    // Remove newline from time string
    time_str[strcspn(time_str, "\n")] = 0;

    // Write log entry
    fprintf(log_file, "Username: %s | Action: %s | Timestamp: %s\n",
            username, action_type, time_str);

    // Close the file
    fclose(log_file);

    return 0;
}