#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beautiful_cli.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Advanced CLI Techniques
void create_progress_bar(int progress, int total)
{
    const int bar_width = 50;
    float percentage = (float)progress / total;
    int filled_width = percentage * bar_width;

    printf(ANSI_BOLD ANSI_COLOR_CYAN "[" ANSI_COLOR_RESET);
    for (int i = 0; i < bar_width; i++)
    {
        if (i < filled_width)
            printf(ANSI_COLOR_GREEN "=" ANSI_COLOR_RESET);
        else
            printf(" ");
    }
    printf(ANSI_BOLD ANSI_COLOR_CYAN "] %.1f%%\r" ANSI_COLOR_RESET, percentage * 100);
    fflush(stdout);
}

// Animated Loading Spinner
void animated_spinner(int frames)
{
    const char *spinner_chars = "|/-\\";
    for (int i = 0; i < frames; i++)
    {
        printf(ANSI_COLOR_YELLOW "%c\r" ANSI_COLOR_RESET, spinner_chars[i % 4]);
        fflush(stdout);
        usleep(200000); // 200ms delay
    }
    printf("\r" ANSI_COLOR_GREEN "Done!     \n" ANSI_COLOR_RESET);
}

// Text Typewriter Effect
void typewriter_effect(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(50000); // 50ms delay between characters
    }
    printf("\n");
}