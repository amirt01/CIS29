/******************************
 * Name: Amir Tadros          *
 * Assignment # 2             *
 * Compiler: CodeBlocks 20.03 *
 * OS: Windows 10             *
 *****************************/

#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <algorithm>

#include "date.hpp"
#include "score.hpp"

using namespace std;

const char ScoresFile[] = "ass2scoresfile.dat";
const char InputFile[] = "ass2data.txt";
const int MaxNumberOfScores = 10;

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
    return(Score(cleanName, stoi(score), Date(date)));
}

void eraseScoresFile()
{
    ofstream fout;

    if (!fout)
    {
        cout << "Error in opening file...\n";
        exit (EXIT_FAILURE);
    }

    fout.open(ScoresFile, ofstream::out | ofstream::trunc);
    fout.close();
}

int numOfLines(const char* fileName)
{
    int numOfScores = 0;
    ifstream fin(fileName);

    if (!fin)
    {
        cout << "Error in opening file...\n";
        exit (EXIT_FAILURE);
    }

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
        exit (EXIT_FAILURE);
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
        exit (EXIT_FAILURE);
    }

    int i = 0;
    while (fin.read(reinterpret_cast<char*>(&scores[i]), sizeof(Score)))
        i++;

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
        exit (EXIT_FAILURE);
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
                cout << left << setw(3) << i + 1 << scores[i];
            cout << "----------------------------------\n";

            writeScoresToFile(scores, numOfScores);
        }
    }

    fin.close();

    return(0);
}
