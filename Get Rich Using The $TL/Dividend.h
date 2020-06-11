#include <ostream>
#include <string>
#include <ctime>

#include "Date.h"

class Dividend
{
private:
	Date date;
	double div;
public:
	Dividend(Date date, double div = 0.0)
		: date(date), div(div) {}
    double getDiv() const { return div; };
    const Date getDate() const { return date; };
	friend bool operator<(const Dividend& d1, const Dividend& d2) { return (d1.date < d2.date); };
	friend std::ostream& operator<<(std::ostream& out, const Dividend& d);
};
