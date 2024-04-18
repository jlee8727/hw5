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
void generateWords(string currentWord, int index, const string& in, const string& floating, const set<string>& dict, set<string>& results, int blanks);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in, //input
    const std::string& floating, //yellow
    const std::set<std::string>& dict) //dictionary
{
  // Add your code here
  set<string> results;
  string currentWord = in;
  int blanks = 0;
  for(int i = 0; i < in.size(); i++){
    if(in[i] == '-'){
      blanks++;
    }
      
  }
  generateWords(currentWord, 0, in, floating, dict, results, blanks);

  return results;
}

// Define any helper functions here
void generateWords(string currentWord, int index, const string& in, const string& floating, const set<string>& dict, set<string>& results, int blanks){

  if (index == in.size()) {
    if (dict.find(currentWord) != dict.end() && floating == "") {
      results.insert(currentWord);
    }
    return;
  }

  if (in[index] == '-') {
    if(blanks == floating.size()){
      string newFloating = floating;
      for(int i = 0; i < floating.size(); i++){
        currentWord[index] = newFloating[i];
        blanks--;
        std::string::iterator it = find(newFloating.begin(), newFloating.end(), floating[i]);
        if (it != newFloating.end()) newFloating.erase(it);
        generateWords(currentWord, index + 1, in, newFloating, dict, results, blanks);
        blanks++;
        newFloating = floating;
      }
    } 
    else {
      for (char ch = 'a'; ch <= 'z'; ch++) {
        string newFloating = floating;
        currentWord[index] = ch;
        std::string::iterator it = find(newFloating.begin(), newFloating.end(), ch);
        if (it != newFloating.end()) newFloating.erase(it); // Remove one occurrence of 'ch'
        blanks--;
        generateWords(currentWord, index + 1, in, newFloating, dict, results, blanks);
        newFloating = floating;
        currentWord[index] = '-';  // Reset for the next iteration
        blanks++;

      }
    }
  } 
  else { //has a letter already
    generateWords(currentWord, index + 1, in, floating, dict, results, blanks);
  }
}

