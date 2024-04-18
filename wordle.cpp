#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
std::set<std::string> dfs(std::string& in, std::string floating, const std::set<std::string>& dict, size_t, int);
bool isValid(const std::string& word, const std::string& floating, const std::set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    string inCopy = in;
    int numSpace = 0;
    for (char c : in)
    {
        if (c == '-')
        {
            numSpace++;
        }
    }
    return dfs(inCopy, floating, dict, numSpace, 0);
}

// Define any helper functions here
bool isValid(const std::string& word, const std::string& floating, const std::set<std::string>& dict) {
    return dict.find(word) != dict.end();
}

std::set<std::string> dfs(std::string& in, string floating, const std::set<std::string>& dict, size_t numSpace, int usedFloating)
{
    // We can use discretization (vector storing the index of the '-'s in the string) to optimize
    // but there are only a few '-' and the guide didn't say we can't use string::find
    // so I'm lazy
    size_t pos = in.find('-');
    // Base case
    // no '-'
    if (pos == string::npos)
    {
        if (isValid(in, floating, dict))
        {
            return {in};
        }
        else
        {
            return {};
        }
    }
    // Recursive case
    set <string> result;
    if (numSpace > floating.size() - usedFloating) {
        for (char c = 'a'; c <= 'z'; c++) {
            // can instead use a used array to optimize
            // but I'm lazy
            size_t fpos = floating.find(c);
            in[pos] = c;
            char used_f = floating[fpos];
            if (fpos != string::npos) {
                floating[fpos] = '-';
                usedFloating++;
            }
            set<string> temp = dfs(in, floating, dict, numSpace - 1, usedFloating);
            result.insert(temp.begin(), temp.end());
            if (fpos != string::npos) {
                usedFloating--;
                floating[fpos] = used_f;
            }
        }
    }
    // if we must use floating characters
    // extracted to optimize efficiency
    else {
        for (size_t fpos = 0; fpos < floating.size(); fpos++) {
            if (floating[fpos] == '-') {
                continue;
            }
            in[pos] = floating[fpos];
            char used_f = floating[fpos];
            floating[fpos] = '-';
            usedFloating++;
            set<string> temp = dfs(in, floating, dict, numSpace - 1, usedFloating);
            result.insert(temp.begin(), temp.end());
            usedFloating--;
            floating[fpos] = used_f;
        }
    }
    in[pos] = '-';
    return result;
}