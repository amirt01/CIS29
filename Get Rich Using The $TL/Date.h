#include <ostream>
#include <string>
#include <ctime>

class Date
{
private:

public:
    time_t date;
	Date() : date(time(0)) {}
	Date(const Date&) = default;
	Date(time_t date) : date(date) {} // Date in time_t format
	Date(const std::string RawDate);
	int getYear() const;
	time_t getDate() const;
	friend bool operator<(const Date& d1, const Date& d2) { return (d1.date < d2.date); };
	friend std::ostream& operator<<(std::ostream& out, const Date& d);
};
