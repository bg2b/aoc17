// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

void redistribute(vector<int> &banks) {
  auto bank = max_element(banks.begin(), banks.end());
  int blocks = *bank;
  *bank = 0;
  while (blocks > 0) {
    ++bank;
    if (bank == banks.end())
      bank = banks.begin();
    ++*bank;
    --blocks;
  }
}

void solve(bool cycle_length) {
  vector<int> banks;
  int bank;
  while (cin >> bank)
    banks.push_back(bank);
  map<vector<int>, int> seen;
  int steps = 0;
  while (!seen.count(banks)) {
    seen.emplace(banks, steps);
    redistribute(banks);
    ++steps;
  }
  if (cycle_length)
    steps -= seen[banks];
  cout << steps << '\n';
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
