#include "Date.h"

Date::Date(std::string RawDate)
    : date(time(0))
{
    tm d = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    d.tm_isdst = -1;

    std::string buffer;

    d.tm_year = std::stoi(RawDate.substr(0,4)) - 1900;
    d.tm_mon = std::stoi(RawDate.substr(5, 2)) - 1;
    d.tm_mday = std::stoi(RawDate.substr(8,2));

    date = mktime(&d);
}

int Date::getYear() const
{
    return localtime(&date)->tm_year;
}

time_t Date::getDate() const
{
    return date;
}

std::ostream& operator<<(std::ostream& output, const Date& d)
{
    tm* dateInfo = localtime(&d.date);
    char buffer [32];

    strftime (buffer, 80, "%m/%d/%Y", dateInfo);
    output << buffer;
    return output;
}
