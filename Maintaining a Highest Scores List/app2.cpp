#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

const char ScoresFile[] = "ass2scoresfile.dat";
const char InputFile[] = "ass2data.txt";
const int MaxNumberOfScores = 10;

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

            const char months[12][4] = { "jan", "feb", "mar", "apr", "may", "jun",
                                         "jul", "aug", "sep", "oct", "nov", "dec" };

            for (int i = 0; i < 12; i++)
                if (strcmp(buffer, months[i]) == 0)
                    d.tm_mon = i;
        }
        date = mktime(&d);
    }
    else if (isdigit(RawDate[2]))
        date = stoi(RawDate); // Already in the correct format
    else
        date = time(0);
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
    Score(){};
    Score(char*, int, Date);
    char* getName(){ return name; };
    int getScore(){ return score; };
    Date getDate(){ return date; };
    friend ostream& operator<<(ostream&, const Score&);
    friend bool operator<(const Score&, const Score&);
};

Score::Score(char* n, int s, Date d)
{
    strcpy(name, n);
    score = s;
    date = d;
}

bool operator<(const Score& a, const Score& b)
{
    return a.score < b.score ;
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
        else if (isalpha(RawDate[2]))// ddmmmyy
        {
            strncpy(buffer,RawDate, 2);
            day = stoi(buffer);
            strncpy(buffer, &RawDate[5], 2);
            year = stoi(buffer);
            strncpy(buffer, &RawDate[2], 3);

            for (char &c : buffer)
                c = tolower(c);

            const char months[12][4] = { "jan", "feb", "mar", "apr", "may", "jun",
                                         "jul", "aug", "sep", "oct", "nov", "dec" };

            for (int i = 0; i < 12; i++)
                if (strcmp(buffer, months[i]) == 0)
                    month = i;
        }
        else
            return Date(time(0));
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

ostream& operator<<(ostream &output, const Score &s)
{
    output << setw(19) << s.name << setw(2) << s.score << "  " << setw(8) << s.date << endl;
    return output;
}

Score processInputFile(ifstream &fin){
    char* name = new char[16];
    char* date = new char[13];
    char* score = new char[2];
    int nameLen = 15;

    fin.read(name, 16);
    fin.read(date, 13);
    fin.read(score, 3);

    while(nameLen > 0)
    {
        if (isblank(name[nameLen]))
            name[nameLen] = 0;
        else
            break;
        nameLen--;
    }

    for (int i = 13; i > 0; i--)
    {
        if (isblank(date[i]))
            date[i] = 0;
        else
            break;
    }

    score[2] = 0;

    char* cleanName = new char[nameLen]; // solve bug in copying
    strcpy(cleanName, name);             // char pointer to char array
    delete name;
    return(Score(cleanName, stoi(score), processDate(date)));
}

void eraseScoresFile()
{
    ofstream fout;
    fout.open(ScoresFile, ofstream::out | ofstream::trunc);
    fout.close();
}

int numOfLines(const char* fileName)
{
    int numOfScores = 0;
    ifstream fin(fileName);
    numOfScores = count(istreambuf_iterator<char>(fin), istreambuf_iterator<char>(), '\n');
    fin.close();
    return(numOfScores);
}

void writeScoresToFile(Score scores[MaxNumberOfScores], int numOfScores)
{
    ofstream fout;
    fout.open(ScoresFile, ios::trunc | ios::out | ios::binary);
    if(!fout)
    {
        cout << "Error in opening file...\n";
    }

    for (int i = 0; i < numOfScores; i++)
    {
        fout.write(reinterpret_cast<char*>(&scores[i]), sizeof(scores[i]));
    }
    fout.close();
}

void sortScores(Score scores[], int numOfScores) {
    Score temp;
    for (int i = 0; i < numOfScores - 1; i++) {
        int max = i;
        for (int j = i + 1; j < numOfScores; j++)
            if (scores[max] < scores[j])
        max = j;
        temp = scores[i];
        scores[i] = scores[max];
        scores[max] = temp;
    }
}

int getScoresFromFile(Score* scores)
{
    ifstream fin;
    fin.open(ScoresFile, ios::in | ios::binary);
    if (!fin)
    {
        cout << "Error in opening file...\n";
    }
    int i = 0;
    while (fin.read(reinterpret_cast<char*>(&scores[i]), sizeof(Score)))
    {
        i++;
    }
    return i;
}

int main()
{
    Score scores[MaxNumberOfScores];
    int numOfScores = 0;
    Score newScore;
    bool updateScores = false;

    ifstream fin(InputFile);
    if (!fin)
    {
        cout << "Unable to open input file " << InputFile << endl;
        exit(1);
    }

    eraseScoresFile();

    for (int i = 0; i < numOfLines(InputFile) + 1; i++)
    {
        numOfScores = getScoresFromFile(scores);
        newScore = processInputFile(fin);

        if(numOfScores < MaxNumberOfScores)
        {
            scores[numOfScores++] = newScore;
            updateScores = true;
        }

        else if (scores[numOfScores - 1] < newScore)
        {
            scores[numOfScores - 1] = newScore;
            updateScores = true;
        }

        else
            updateScores = false;

        if (updateScores)
        {
            sortScores(scores, numOfScores);
            for (int i = 0; i < numOfScores; i++)
                cout << left << setw(3) << i + 1 << scores[i] << endl;
            cout << "----------------------------------\n";

            writeScoresToFile(scores, numOfScores);
        }
    }

    fin.close();

    return(0);
}
