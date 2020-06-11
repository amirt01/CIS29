/******************************
 * Name: Amir Tadros          *
 * Assignment # 5             *
 * Compiler: CodeBlocks 20.03 *
 * OS: Windows 10             *
 *****************************/

#include "MutualFund.h"
#include "Date.h"
#include "Dividend.h"

MutualFund::MutualFund(const std::string ticker)
    : ticker(ticker), navHistory(), divHistory()
{
	readNavHistory(ticker + "shuffled.csv");
	std::cout << std::endl;
	readDivHistory(ticker + "div.csv");
    std::cout << std::endl;
	writeNavHistory(ticker + "nav.txt");
    std::cout << std::endl;
	writeDivHistory(ticker + "div.txt");
	std::cout << std::endl;
}

void MutualFund::readNavHistory(const std::string path)
{
    std::string temp;
	std::ifstream fin;
	fin.open(path);

	if (!fin.is_open())
	{
		std::cout << "Error opening input file with path: " << path << std::endl;
		exit(EXIT_FAILURE);
	}

    std::cout << "Reading file " << path << " into map<Date,float>..." << std::endl;

	while (std::getline(fin, temp))
		navHistory[Date(temp.substr(0, 10).c_str())] = std::stof(temp.substr(41, 9));

    std::cout << navHistory.size() << " records read" << std::endl;

	fin.close();
}

void MutualFund::readDivHistory(const std::string path)
{
    std::string temp;
	std::ifstream fin;
	fin.open(path);

	if (!fin.is_open())
	{
		std::cout << "Error opening input file with path: \"" << path << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	fin.ignore(256, '\n'); // Throw away the first line of the input file

	std::cout << "Reading file " << path << " into set<Dividend>..." << std::endl;

	while (std::getline(fin, temp))
	{

		divHistory.insert(Dividend(Date(temp.substr(0, 10).c_str()), std::stof(temp.substr(11, 9))));
	}

	std::cout << divHistory.size() << " records read" << std::endl;

	fin.close();
}

void MutualFund::writeNavHistory(const std::string path) const
{
	std::ofstream fout;
	fout.open(path);

	if (!fout.is_open())
	{
		std::cout << "Error opening output file with path: \"" << path << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

    std::cout << "Writing NAV history data into file " << path << "...";

	for (const auto& kv : navHistory)
		fout << kv.first << "     " << kv.second << "\n";

    std::cout << "done";

	fout.close();
}

void MutualFund::writeDivHistory(const std::string path) const
{
	std::ofstream fout;
	fout.open(path);

	if (!fout.is_open())
	{
		std::cout << "Error opening output file with path: \"" << path << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Writing dividend history data into file " << path << "...";

	for (const auto& div : divHistory)
		fout << div << "\n";

    std::cout << "done" << std::endl;

	fout.close();
}

std::map<Date, float>::iterator MutualFund::getFirstOfYear(const int year)
{
    std::map<Date, float>::iterator it = navHistory.begin();
    while (it->first.getYear() < year) { ++it; }
    return it;
}

double MutualFund::getMinOfLastYear()
{
    return std::min_element(getFirstOfYear(navHistory.rbegin()->first.getYear()),
                                           --navHistory.end(),
                                           [](const auto& l, const auto& r) { return l.second < r.second; })->second;
}

double MutualFund::getMaxOfLastYear()
{
    return std::max_element(getFirstOfYear(navHistory.rbegin()->first.getYear()),
                                           --navHistory.end(),
                                           [](const auto& l, const auto& r) { return l.second < r.second; })->second;
}

double MutualFund::getNDayAverage(const std::size_t days)
{
    if (days > navHistory.size())
    {
        std::cout << "There aren't enough data points to take a " << days << "day average." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::map<Date, float>::iterator it = navHistory.begin();
    std::advance(it, navHistory.size()-days);
    return std::accumulate(it, navHistory.end(), 0.0,
                    [](const double previous, const auto& element) { return previous + element.second; }) /
                       static_cast<double>(days);
}

double MutualFund::getGainOverNDayAverage(const std::size_t days)
{
    return (navHistory.rbegin()->second - getNDayAverage(days)) / getNDayAverage(days) * 100;
}

float MutualFund::get3YearAgo()
{
    const float initial = 10000.f;
    float shares = 0.f;

    struct tm * timeinfo;
    time_t date = navHistory.rbegin()->first.getDate();
    timeinfo = localtime (&date);
    timeinfo->tm_year -= 3;
    time_t navDate = mktime(timeinfo);
    time_t divDate = navDate + 86400 * 30;

    if (navHistory.find(Date(navDate)) != navHistory.end())
        shares += initial / navHistory.at(Date(navDate));
    else
        shares += initial / navHistory.upper_bound(Date(navDate))->second;

    std::ofstream fout("dividend_detail.txt", std::ios::app);

    if (!fout.is_open())
    {
        std::cout << "Error opening dividend_detail.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    Date startDate;
    Date endDate = navHistory.crbegin()->first;

    if (navHistory.find(Date(navDate)) != navHistory.end())
        startDate = navHistory.find(Date(navDate))->first;
    else
        startDate =navHistory.upper_bound(Date(navDate))->first;

    fout << "Dividends for " << ticker << ": " << startDate << '-' << endDate << std::endl;

    fout << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << std::endl;

    auto divCalc = [&](const auto& element) {
        fout << std::fixed << std::setprecision(3) << element.getDate() << "    ";
        fout << std::fixed << std::setprecision(3) << element.getDiv() << "   ";
        fout << std::fixed << std::setprecision(3) << shares << "    ";
        fout << std::fixed << std::setprecision(2) << shares * element.getDiv() << "   ";
        fout << std::fixed << std::setprecision(2) << navHistory.find(element.getDate())->second << "    ";
        fout << std::fixed << std::setprecision(3) << shares * element.getDiv() / navHistory.find(element.getDate())->second << "       ";
        shares += shares * element.getDiv() / navHistory.find(element.getDate())->second;
        fout << std::fixed << std::setprecision(3) << shares << std::endl;
    };

    if (divHistory.find(Dividend(divDate)) != divHistory.end())
        std::for_each(divHistory.find(Dividend(divDate)), divHistory.end(), divCalc);
    else
        std::for_each(divHistory.upper_bound(Dividend(divDate)), divHistory.end(), divCalc);

    fout << std::endl;

    fout.close();

    return (shares * navHistory.crbegin()->second - initial) / initial * 100.f;
}

float MutualFund::getTimePeriod(const time_t& navDate, const int& days)
{
    const float initial = 10000.f;
    const time_t divDate = navDate + 86400 * 30;
    const time_t endDate = navDate + 86400 * days;

    float shares = 0.f;

    auto navItEnd = --navHistory.lower_bound(Date(endDate));
    auto navItBegin = navHistory.find(Date(navDate));
    if (navItBegin == navHistory.end())
        navItBegin = navHistory.upper_bound(Date(navDate));

    auto divItEnd = divHistory.lower_bound(Dividend(endDate));
    auto divItBegin = divHistory.find(Dividend(divDate));
    if (divItBegin == divHistory.end())
        divItBegin = divHistory.upper_bound(Dividend(divDate));
    shares += initial / navItBegin->second;

    std::ofstream fout("dividend_detail.txt", std::ios::app);

    if (!fout.is_open())
    {
        std::cout << "Error opening dividend_detail.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    fout << "Dividends for " << ticker << ": " << navItBegin->first << '-' << navItEnd->first << std::endl;

    fout << "Date      Dividend   Shares    Div Amt  Sh Price Purchased  New Sh Owned" << std::endl;
    std::for_each(divItBegin, divItEnd,
        [&](const auto& element) {
            fout << std::fixed << std::setprecision(3) << element.getDate() << "    ";
            fout << std::fixed << std::setprecision(3) << element.getDiv() << "   ";
            fout << std::fixed << std::setprecision(3) << shares << "    ";
            fout << std::fixed << std::setprecision(2) << shares * element.getDiv() << "   ";
            fout << std::fixed << std::setprecision(2) << navHistory.find(element.getDate())->second << "    ";
            fout << std::fixed << std::setprecision(3) << shares * element.getDiv() / navHistory.find(element.getDate())->second << "       ";

            shares += shares * element.getDiv() / navHistory.find(element.getDate())->second;

            fout << std::fixed << std::setprecision(3) << shares << std::endl;
        }
    );
    fout << std::endl;

    fout.close();

    std::cout << std::setw(10) << std::right << shares * navItEnd->second << " ";
    std::cout << std::setw(10) << std::right << shares * navItEnd->second - 10000 << "  ";

    return shares * navItEnd->second;
}

void MutualFund::report()
{
    std::cout << "Fund: " << ticker << std::endl;
    std::cout << "Analysis Date: " << navHistory.crbegin()->first << std::endl;
    std::cout << "Latest NAV: $" << navHistory.crbegin()->second << std::endl;
    std::cout << "Minimum for the last year = $" << getMinOfLastYear() << std::endl;
    std::cout << "Maximum for the last year = $" << getMaxOfLastYear() << std::endl;
    std::cout << "50 day moving average = $" << std::fixed << std::setprecision(2) << getNDayAverage(50) << std::endl;
    std::cout << "200 day moving average = $" << std::fixed << std::setprecision(2) << getNDayAverage(200) << std::endl;
    std::cout << "% gain over 200 Day Moving Avg: " << std::fixed << std::setprecision(2) << getGainOverNDayAverage(200) << '%' << std::endl;
    std::cout << "% return over the last 3 years: " << get3YearAgo() << '%' << std::endl;
    std::cout << std::endl;
}


void readInFile(const std::string path, std::unordered_set<std::string>& uSet)
{
	std::ifstream fin;
	std::string temp;
	fin.open(path);

	if (!fin.is_open())
	{
		std::cout << "Error opening input file with path: \"" << path << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

    std::cout << "Reading " << path << " into an unordered_set<string> ..." << std::endl;

	fin.ignore(256, '\n'); // Throw away the first line of the input file

	while (std::getline(fin, temp))
	{
		uSet.insert(temp);
	}
	std::cout << "Input data stored in " << uSet.size() << " buckets" << std::endl;

	fin.close();
}

void writeToFile(const std::string path, const std::unordered_set<std::string> uSet)
{
	std::ofstream fout;
	fout.open(path);

	if (!fout.is_open())
	{
		std::cout << "Error opening output file with path: \"" << path << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Writing unordered_set<string> data into file " << path << std::endl << std::endl;

	for (std::string str : uSet)
		fout << str << "\n";

	fout.close();
}

void analyzeFunds(MutualFund& VDADX, MutualFund& VBIAX)
{
    std::mt19937 rng(time(0));

    double VDADXTotalAvg = 0.0;
    double VBIAXTotalAvg = 0.0;

    std::cout << "  Buy Date    Sell Date   VDADX Value  VDADX Gain VBIAX Value  VBIAX Gain   VDADX-VBIAX" << std::endl;

    for (int i = 0; i < 100; i++) {
        std::uniform_int_distribution<int> randStart(VDADX.getFirstDate(), VDADX.getLastDate() - 86400 * 365);
        time_t rStart = randStart(rng);
        std::uniform_int_distribution<int> randLength(365, (VDADX.getLastDate() - rStart) / 86400);
        int rLength = randLength(rng);

        std::cout << VDADX.getBuyDate(rStart) << "    " << VDADX.getSellDate(rStart + rLength * 86400) << "  ";

        int VDADXInt = VDADX.getTimePeriod(rStart, rLength);
        int VBIAXInt = VBIAX.getTimePeriod(rStart, rLength);

        std::cout << "  " << std::setw(10) << std::right << VDADXInt-VBIAXInt;

        VDADXTotalAvg += VDADXInt;
        VBIAXTotalAvg += VBIAXInt;

        std::cout << std::endl;
    }

    std::cout << "Average gain for VDADX = " << std::fixed << std::setprecision(2) << VDADXTotalAvg / 100.0 - 10000.0 << std::endl;
    std::cout << "Average gain for VBIAX = " << VBIAXTotalAvg / 100.0 - 10000.0 << std::endl;
}

int main()
{
	std::unordered_set<std::string> VBIAX;
	std::unordered_set<std::string> VDADX;

	readInFile("VBIAX.csv", VBIAX);
	writeToFile("VBIAXshuffled.csv", VBIAX);
	readInFile("VDADX.csv", VDADX);
	writeToFile("VDADXshuffled.csv", VDADX);

	MutualFund vdadx("VDADX");
	MutualFund vbiax("VBIAX");

	vdadx.report();
	vbiax.report();

    analyzeFunds(vdadx, vbiax);

	return EXIT_SUCCESS;
}
