#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <map>

using namespace std;

const char InputFile[] = "ass2data.txt";
int MaxNumberOfScores = 32;

class Date {
private:
    time_t date;
public:
    Date(){ date = time(0); }
    //Date(const Date&) = default;
    Date(time_t d) { date = d; } // Date in time_t format
    Date(const char*);
    friend ostream& operator<<(ostream&, const Date&);
};
//Date::Date(const Date&) {}
Date::Date(const char* RawDate)
{
    int year, month, day;

    tm d = {0};
    d.tm_isdst = -1;

    char buffer[32];
    if (!isdigit(RawDate[2]))
    {
        if (ispunct(RawDate[2])) // Convert from mm/dd/yy or mm-dd-yy
        {
            strncpy(buffer, RawDate, 2);
            d.tm_mon = stoi(buffer);
            strncpy(buffer, &RawDate[3], 2);
            d.tm_mday = stoi(buffer);
            strncpy(buffer, &RawDate[6], 2);
            d.tm_year = stoi(buffer) + 100;
        }
        else // Convert from ddmmmyy
        {
            strncpy(buffer,RawDate, 2);
            d.tm_mday = stoi(buffer);
            strncpy(buffer, &RawDate[5], 2);
            d.tm_year = stoi(buffer) + 100;
            strncpy(buffer, &RawDate[2], 3);

            for (char &c : buffer)
                c = tolower(c);

            const char months[12][4] = { "jan",
                                         "feb",
                                         "mar",
                                         "apr",
                                         "may",
                                         "jun",
                                         "jul",
                                         "aug",
                                         "sep",
                                         "oct",
                                         "nov",
                                         "dec" };

            for (int i = 0; i < 12; i++)
                if (strcmp(buffer, months[i]) == 0)
                    d.tm_mon = i;
        }
        date = mktime(&d);
    }
    else
        date = stoi(RawDate); // Already in the correct format
}

ostream& operator<<(ostream &output, const Date &d)
{
    struct tm* dateInfo = localtime(&d.date);
    char buffer [32];

    dateInfo->tm_mon;
    strftime (buffer, 80, "%m/%d/%y", dateInfo);
    output << buffer;
    return output;
}

class Score {
private:
    char name[16];
    int score;
    Date date;
public:
    Score();
    Score(char*, int, Date);
    char* getName(){ return name; };
    int getScore(){ return score; };
    Date getDate(){ return date; };
    //Score operator+(const Score& s);
};

Score::Score(char* n, int s, Date d)
{
    strcpy(name, n);
    score = s;
    date = d;
}

Date processDate(char* RawDate)
{
    int year, month, day;
    char buffer[32];
    if (!isdigit(RawDate[2]))
    {
        if (ispunct(RawDate[2])) // mm/dd/yy or mm-dd-yy
        {
            strncpy(buffer, RawDate, 2);
            month = stoi(buffer);
            strncpy(buffer, &RawDate[3], 2);
            day = stoi(buffer);
            strncpy(buffer, &RawDate[6], 2);
            year = stoi(buffer);
        }
        else // ddmmmyy
        {
            strncpy(buffer,RawDate, 2);
            day = stoi(buffer);
            strncpy(buffer, &RawDate[5], 2);
            year = stoi(buffer);
            strncpy(buffer, &RawDate[2], 3);

            for (char &c : buffer)
                c = tolower(c);

            const char months[12][4] = { "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

            for (int i = 0; i < 12; i++)
                if (strcmp(buffer, months[i]) == 0)
                    month = i;
        }
    }
    else
        return(stoi(RawDate));

    tm d = {0};
    d.tm_isdst = -1;
    d.tm_year = year + 100;
    d.tm_mon = month;
    d.tm_mday = day;

    return(Date(mktime(&d)));
}

Score processInputFile(ifstream &fin){
    char* name = new char[16];
    char date[13];
    int score;

    fin.read(name, 16);
    fin >> date >> score;

    for (int i = 16; i > 0; i--)
    {
        if (isblank(name[i]))
            name[i] = 0;
        else
            break;
    }

    return(Score(name, score, processDate(date)));
}

int main()
{
//    Score scores[MaxNumberOfScores];

    ifstream fin(InputFile);
    if (!fin)
    {
        cout << "Unable to open input file " << InputFile << endl;
        exit(1);
    }

    Score score = processInputFile(fin);

    tm * timeinfo;

    cout << score.getDate();

    return(0);
}
