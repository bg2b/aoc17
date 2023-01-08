// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

void solve(bool anagrams) {
  int num_valid = 0;
  string passphrase;
  while (getline(cin, passphrase)) {
    bool valid = true;
    set<string> words;
    stringstream ss(passphrase);
    string word;
    while (ss >> word) {
      if (anagrams)
        sort(word.begin(), word.end());
      if (!words.emplace(word).second)
        valid = false;
    }
    if (valid)
      ++num_valid;
  }
  cout << num_valid << '\n';
}

void part1() { solve(false); }
void part2() { solve(true); }

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " partnum < input\n";
    exit(1);
  }
  if (*argv[1] == '1')
    part1();
  else
    part2();
  return 0;
}
