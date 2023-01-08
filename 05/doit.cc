// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>

using namespace std;

void solve(bool part2) {
  vector<int> offsets;
  int offset;
  while (cin >> offset)
    offsets.push_back(offset);
  int num_steps = 0;
  int pc = 0;
  while (pc >= 0 && pc < int(offsets.size())) {
    int next = pc + offsets[pc];
    offsets[pc] += part2 && offsets[pc] >= 3 ? -1 : +1;
    pc = next;
    ++num_steps;
  }
  cout << num_steps << '\n';
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
