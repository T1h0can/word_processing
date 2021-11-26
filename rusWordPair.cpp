#include <iostream>
#include <map>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cctype>
#include "utf8/utf8.h"

using namespace std;

typedef int UnicodeChar;
typedef basic_string <UnicodeChar> UnicodeString;
typedef pair <UnicodeString, UnicodeString> wordPair;

bool compareUnicodeStrings(
        const UnicodeString &s0, const UnicodeString &s1
);

bool comparePairs(
        const pair<UnicodeString, int> &i0,
        const pair<UnicodeString, int> &i1
);

bool compare(const pair<wordPair, int> &i0, const pair<wordPair, int> &i1);

std::ostream &operator<<(std::ostream &stream, const UnicodeString &str);

int main() {
    map<wordPair, int> wordPairs;
    UnicodeString word;
    deque <UnicodeString> wordsDeque;    //deque for pair of words
    bool readingWord = false;
    while (true) {
        bool readFail;
        UnicodeChar c = UnicodeChar(get_utf8_code_point(cin, readFail));
        if (readFail)
            if (!cin.good())
                break;
        if (readingWord) {
            if (isRussianLetter(c)) {
                c = toLowerLetter(c);
                word += c;
            } else {
                readingWord = false;
                if (c == ' ' || c == '-' || c == '\n') {
                    wordsDeque.push_back(word);
                    if (wordsDeque.size() == 2) {
                        wordPair pair_(wordsDeque.front(), wordsDeque.back());
                        wordsDeque.pop_front();             //always keep last one word in deque
                        if (wordPairs.count(pair_) == 0)    //check pair in map
                            wordPairs[pair_] = 1;
                        else
                            ++(wordPairs[pair_]);
                    } else {
//                        word.clear();
                        continue;
                    }
                }
            }
        } else {
            if (isRussianLetter(c)) {
                readingWord = true;
                word.clear();
                c = toLowerLetter(c);
                word += c;
            }
        }
    }
    vector <pair<wordPair, int>> pairFreq;
//    cout << wordPairs.size() << endl;
    for (auto i = wordPairs.cbegin(); i != wordPairs.cend(); ++i) {
//        cout << "<" << i->first.first << ", " << i->first.second << ">, " << i->second << endl;
        pairFreq.push_back(*i);
    }
    sort(pairFreq.begin(), pairFreq.end(), compare);
    cout << "Number of different pair of words in text: " << pairFreq.size() << endl;
    auto size = (pairFreq.size() > 500) ? 500 : pairFreq.size();
    for (int i = 0; i <size; ++i)
        cout << "<" << pairFreq[i].first.first << ", " << pairFreq[i].first.second << ">\t" << pairFreq[i].second << endl;
}


bool compare(const pair<wordPair, int> &i0, const pair<wordPair, int> &i1) {
    return (i0.second > i1.second ||
            (i0.second == i1.second &&
             (compareUnicodeStrings(i0.first.first, i1.first.first) ||
              (i0.first.first == i1.first.first && compareUnicodeStrings(i0.first.second, i1.first.second)))));
}


bool comparePairs(
        const pair<UnicodeString, int> &i0,
        const pair<UnicodeString, int> &i1
) {
    return (
            i0.second > i1.second ||
            (i0.second == i1.second &&
             compareUnicodeStrings(i0.first, i1.first))
    );
}

bool compareUnicodeStrings(
        const UnicodeString &s0, const UnicodeString &s1
) {
    int len0 = s0.length();
    int len1 = s1.length();
    for (int i = 0; i < len0; ++i) {
        if (i >= len1)
            return false;
        int c = compareRussianLetters(s0[i], s1[i]);
        if (c < 0)
            return true;
        else if (c > 0)
            return false;
    }
    return (len0 < len1);
}

std::ostream &operator<<(std::ostream &stream, const UnicodeString &str) {
    for (size_t i = 0; i < str.length(); ++i) {
        output_utf8(stream, str[i]);
    }
    return stream;
}
