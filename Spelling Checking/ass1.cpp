/******************************
 * Name: Amir Tadros          *
 * Assignment # 1             *
 * Compiler: CodeBlocks 20.03 *
 * OS: Windows 10             *
 *****************************/

#include <cstring>

#include <iostream>

#include <fstream>

using namespace std;

const char Dict[] = "words";
const char WordFile[] = "gettysburg.txt";
const unsigned short MaxWordSize = 32;

class Word {
  char *word_;

public:
  Word(const char *text = 0);
  ~Word() {
    delete[] word_;
    word_ = nullptr;
  }
  const char *word() const;
};

Word::Word(const char *arg) : word_(new char[strlen(arg) + 1])
{
  strcpy(word_, arg);
}

const char *Word::word() const { return word_; }

class Dictionary {
  Word **words_;
  unsigned int capacity_;
  unsigned int numberOfWordsInDictionary_;
  void resize() {
    capacity_ *= 2;
    Word **resizedDict = new Word *[capacity_];
    for (int i = 0; i < numberOfWordsInDictionary_; i++)
      resizedDict[i] = words_[i];
    delete[] words_;
    words_ = resizedDict;
    cout << "Dictionary resized to capacity: " << capacity_ << endl;
  };
  void addWordToDictionary(char word[]) {
    if (numberOfWordsInDictionary_ == capacity_)
      resize();

    for (short i = 0; i < strlen(word); i++)
      word[i] = tolower(word[i]);

    words_[numberOfWordsInDictionary_] = new Word(word);
    numberOfWordsInDictionary_++;
  };

public:
  Dictionary(const char filename[]);
  ~Dictionary() {
    delete[] words_;
    words_ = nullptr;
  };
  bool find(const char *word);
};

Dictionary::Dictionary(const char filename[])
    : words_(new Word *[8]), capacity_(8), numberOfWordsInDictionary_(0) {
  ifstream fin(filename);

  if (!fin)
    cout << "Failed to open dictionary file!" << endl;

  char buffer[MaxWordSize];
  while (fin.getline(buffer, sizeof(buffer)))
    addWordToDictionary(buffer);
}

bool Dictionary::find(const char word[]) {
  int last = numberOfWordsInDictionary_ - 1, first = 0, middle;
  while (first <= last) {
    middle = (first + last) / 2;

    if (strcmp(words_[middle]->word(), word) == 0)
      return true;

    else if (strcmp(words_[middle]->word(), word) > 0)
      last = middle - 1;

    else
      first = middle + 1;
  }
  return false;
}

char* cleanupWord(char x[]) {
  for (short i = 0; i < strlen(x); i++) {
    x[i] = tolower(x[i]);
    if (ispunct(x[i]))
      x[i] = 0;
  }
  return x;
}

int main() {
  char buffer[MaxWordSize];
  Dictionary Websters(Dict);
  ifstream fin(WordFile);

  if (!fin)
    cout << "Failed to open word file!" << endl;

  cout << "\nSpell checking " << WordFile << "\n\n";

  while (fin >> buffer)
    if (!Websters.find(cleanupWord(buffer)) && strlen(cleanupWord(buffer)) != 0)
      cout << buffer << " not found in the Dictionary\n";

  return 0;
}
