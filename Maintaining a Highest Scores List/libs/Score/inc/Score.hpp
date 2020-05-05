#ifndef SCORE_HPP_INCLUDED
#define SCORE_HPP_INCLUDED

#include "Date.hpp"
#include <ostream>

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
    friend std::ostream& operator<<(std::ostream&, const Score&);
    friend bool operator<(const Score&, const Score&);
};

#endif // SCORE_HPP_INCLUDED
