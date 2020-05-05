#ifndef DATE_HPP_INCLUDED
#define DATE_HPP_INCLUDED

#include <ostream>
#include <fstream>
#include <ctime>

class Date {
private:
    time_t date;
public:
    Date(){ date = time(0); }
    Date(const Date&) = default;
    Date(time_t d) { date = d; } // Date in time_t format
    Date(const char*);
    friend std::ostream& operator<<(std::ostream&, const Date&);
};

#endif // DATE_HPP_INCLUDED
