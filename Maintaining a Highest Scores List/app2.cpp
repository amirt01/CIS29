#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <cstdlib>

#include <stdio.h>

using namespace std;

const char InputFile[] = "ass2data.txt";
int MaxNumberOfScores = 32;

/*class Score {
private:
    char name[16];
    int score;
    Date date;
public:
    Score();
    Score(char name[], int score, Date date);
    getName(){return name;};
    getScore(){return score;};
    getDate(){return date;};
    Score operator+(const Score& s);
};

Score(char name[], int score, Date date)
{
    name = name;
    score = score;
    date = date;
}

Score::Score operator+(const Score& s)
{
    Score score;
    score.name = this->score;
}
*/

time_t processInputFile(ifstream &fin){
    char* tempName = new char[16];
    char tempDate[13];
    int tempScore;

    fin.read(tempName, 16);
    fin >> tempDate >> tempScore;

    for (int i = 16; i-- > 0;)
    {
        if (isblank(tempName[i]))
            tempName[i] = 0;
        else
            break;
    }

    int year, month, day;
    char buffer[32];

    if (!isdigit(tempDate[2]))
        if (ispunct(tempDate[2]))
        {
            strncpy(buffer, tempDate, 2);
            month = stoi(buffer);
            strncpy(buffer, &tempDate[3], 2);
            day = stoi(buffer);
            //strcpy(buffer, "20");

            strncpy(buffer, &tempDate[6], 2);
            year = stoi(buffer);
        }

    delete[] tempName;

    //cout << month << '/' << day << '/' << year << endl;

    tm d = {0};
    d.tm_year = year;
    d.tm_mon = month - 1;
    d.tm_mday = day;

    time_t date = mkdate(&d);



    struct tm * timeinfo;

    timeinfo = localtime(&date);
    printf ("Current local time and date: %s", asctime(timeinfo));

    /*char buf[80];
    strftime(buf, 80,"%m/%d/%y", &date);
    puts(buf);
    */

   /* if (isalpha(tempDate[2]))
//        date.tm_mon =

    else if (isdigit(tempDate[2])


    else

*/

    //cout << tempName << " | " << tempDate << " | " << tempScore << '/' ;

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

    time_t date = processInputFile(fin);

//    printf(asctime(&date));

    return(0);
}
