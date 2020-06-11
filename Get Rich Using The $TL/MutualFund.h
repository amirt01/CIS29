#ifndef PART2_H_INCLUDED
#define PART2_H_INCLUDED

#include <iostream>
#include <map>
#include <set>

#include "Dividend.h"
#include "Date.h"

class MutualFund
{
private:
	const std::string ticker;
	std::map<Date, float> navHistory;
	std::set<Dividend> divHistory;
public:
	MutualFund(const std::string ticker);

	void readNavHistory(const std::string path);
	void readDivHistory(const std::string path);

	void writeNavHistory(const std::string path) const;
	void writeDivHistory(const std::string path) const;

    time_t getFirstDate() { return navHistory.begin()->first.getDate(); }
    time_t getLastDate() { return navHistory.rbegin()->first.getDate(); }
    Date getBuyDate(time_t date) {
        auto navItBegin = navHistory.find(Date(date));
        if (navItBegin == navHistory.end())
            navItBegin = navHistory.upper_bound(Date(date));
        return navItBegin->first;
    }
    Date getSellDate(time_t date) { return (--navHistory.lower_bound(Date(date)))->first; }
    float getValueAtDate(Date date) { return navHistory.at(date); }

	std::map<Date, float>::iterator getFirstOfYear(const int year);
	double getNDayAverage(const std::size_t days);
	double getMinOfLastYear();
	double getMaxOfLastYear();
	double getGainOverNDayAverage(const std::size_t days);
	float get3YearAgo();
    float getTimePeriod(const time_t& navDate, const int& days);

    void report();
};

#endif // PART2_H_INCLUDED
