#include <fstream>
#include <iostream>
#include <cstring>

const char InputFile[] = "ass2dat   a.txt";
int MaxNumberOfScores = 32;


class Score {

};

processInputFile(&fin, &newScore) {

}

int main()
{
    Score scores[MaxNumberOfScores];

    ifstream fin(InputFile);
    if (!fin)
    {
        cerr << "Unable to open input file " << InputFile << endl;
        exit(1);
    }

    std::ofstream ScoresFile;
    ScoresFile.open("ScoresFile.txt", out | trunc | binary);
    //fout.close();

    Score newScore;
    unsigned numScores;
    bool updateScores = false;
    //eraseScoresFile(ScoresFile);

    while (processInputFile(fin, newScore))  // read a record from input file
    {
        // read binary Scores file
        numScores = getScoresFromFile(ScoresFile, scores);

        if (numScores < MaxNumberOfScores)    // Less than 10 Scores
        {
            scores[numScores++] = newScore;
            updateScores = true;
        }
        else if (scores[numScores - 1] < newScore)   // newScore > lowest of Top Scores
        {
            scores[numScores - 1] = newScore;        // add newScore to Top Scores
            updateScores = true;
        }
        else updateScores = false;

        if (updateScores)
        {
            sort(scores, numScores);                 // sort Scores

            for (auto i = 0U; i < numScores; ++i)
                cout << left << setw(3) << i+1 << scores[i] << endl;
            cout << "----------------------------------\n";

            // Write binary Scores file
            writeScoresToFile(ScoresFile, scores, numScores);
        }
    }
}
