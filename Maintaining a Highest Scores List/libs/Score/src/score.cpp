#include "score.hpp"
#include "date.hpp"
#include <cstring>
#include <iomanip>

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

std::ostream& operator<<(std::ostream &output, const Score &s)
{
    output << std::setw(19) << s.name << std::setw(2) << s.score << "  " << std::setw(8) << s.date << std::endl;
    return output;
}
