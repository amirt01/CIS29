#include "Date.hpp"

#include <cstring>
#include <ostream>

Date::Date(const Date&) { date = time(0); }

Date::Date(const char* RawDate)
{
    tm d = {0};
    d.tm_isdst = -1;

    char buffer[32];
    if (!isdigit(RawDate[2]))
    {
        if (ispunct(RawDate[2])) // Convert from mm/dd/yy or mm-dd-yy
        {
            strncpy(buffer, RawDate, 2);
            d.tm_mon = atoi(buffer);
            strncpy(buffer, &RawDate[3], 2);
            d.tm_mday = atoi(buffer);
            strncpy(buffer, &RawDate[6], 2);
            d.tm_year = atoi(buffer) + 100;
        }
        else // Convert from ddmmmyy
        {
            strncpy(buffer,RawDate, 2);
            d.tm_mday = atoi(buffer);
            strncpy(buffer, &RawDate[5], 2);
            d.tm_year = atoi(buffer) + 100;
            strncpy(buffer, &RawDate[2], 3);

            for (char &c : buffer)
                c = tolower(c);

            const char months[12][4] = { "jan", "feb", "mar", "apr", "may", "jun",
                                         "jul", "aug", "sep", "oct", "nov", "dec" };

            for (int i = 0; i < 12; i++)
                if (strcmp(buffer, months[i]) == 0)
                    d.tm_mon = i;
        }
        date = mktime(&d);
    }
    else if (isdigit(RawDate[2]))
        date = atoi(RawDate); // Already in the correct format
    else
        date = time(0);
}

std::ostream& operator<<(std::ostream &output, const Date &d)
{
    struct tm* dateInfo = localtime(&d.date);
    char buffer [32];

    strftime (buffer, 80, "%m/%d/%y", dateInfo);
    output << buffer;
    return output;
}
