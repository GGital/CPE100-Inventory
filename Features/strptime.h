#ifndef _STRPTIME_H
#define _STRPTIME_H

#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Portable strptime implementation
char *strptime(const char *str, const char *format, struct tm *tm)
{
    char *ret;
    char *end;

    // Reset tm structure to prevent garbage values
    memset(tm, 0, sizeof(struct tm));

    while (*format != '\0')
    {
        char c = *format++;

        if (c != '%')
        {
            if (!isspace(*str) && *str != c)
                return NULL;
            while (isspace(*str))
                str++;
            if (*str != c)
                return NULL;
            str++;
            continue;
        }

        c = *format++;
        switch (c)
        {
        case '%': // Literal %
            if (*str++ != '%')
                return NULL;
            break;

        case 'Y': // 4-digit year
            tm->tm_year = strtol(str, &end, 10) - 1900;
            if (end == str)
                return NULL;
            str = end;
            break;

        case 'y': // 2-digit year
        {
            long year = strtol(str, &end, 10);
            if (end == str)
                return NULL;
            // Adjust for 2-digit year (assume 20xx for years 00-69, 19xx for 70-99)
            tm->tm_year = year < 70 ? year + 100 : year;
            str = end;
        }
        break;

        case 'm': // Month (01-12)
        {
            long month = strtol(str, &end, 10);
            if (end == str || month < 1 || month > 12)
                return NULL;
            tm->tm_mon = month - 1;
            str = end;
        }
        break;

        case 'd': // Day of month (01-31)
        {
            long day = strtol(str, &end, 10);
            if (end == str || day < 1 || day > 31)
                return NULL;
            tm->tm_mday = day;
            str = end;
        }
        break;

        case 'H': // Hour (00-23)
        {
            long hour = strtol(str, &end, 10);
            if (end == str || hour < 0 || hour > 23)
                return NULL;
            tm->tm_hour = hour;
            str = end;
        }
        break;

        case 'M': // Minute (00-59)
        {
            long minute = strtol(str, &end, 10);
            if (end == str || minute < 0 || minute > 59)
                return NULL;
            tm->tm_min = minute;
            str = end;
        }
        break;

        case 'S': // Second (00-61 to allow for leap seconds)
        {
            long second = strtol(str, &end, 10);
            if (end == str || second < 0 || second > 61)
                return NULL;
            tm->tm_sec = second;
            str = end;
        }
        break;

        default:
            // Unsupported format specifier
            return NULL;
        }

        // Skip any whitespace after parsing
        while (isspace(*str))
            str++;
    }

    // Normalize the time structure
    mktime(tm);

    return (char *)str;
}

#endif // _STRPTIME_H