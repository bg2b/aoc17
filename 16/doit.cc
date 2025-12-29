// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <cassert>

using namespace std;

using step = function<void(string &line)>;

string start = "abcdefghijklmnop";
vector<step> dance_steps;

void read() {
  string moves;
  cin >> moves;
  moves.push_back(',');
  auto add = [&](step s) { dance_steps.push_back(s); };
  size_t pos = 0;
  while (pos < moves.length()) {
    size_t next_pos = moves.find(',', pos);
    string move = moves.substr(pos, next_pos - pos);
    pos = next_pos + 1;
    if (move[0] == 's') {
      unsigned shift = atoi(move.c_str() + 1);
      assert(shift < start.length());
      add([=](string &s) { rotate(s.rbegin(), s.rbegin() + shift, s.rend()); });
    } else if (move[0] == 'x') {
      unsigned x1 = atoi(move.c_str() + 1);
      unsigned x2 = atoi(move.c_str() + move.find('/') + 1);
      assert(x1 < start.length() && x2 < start.length());
      add([=](string &s) { swap(s[x1], s[x2]); });
    } else {
      assert(move[0] == 'p' && move.length() == 4);
      char c1 = move[1];
      char c2 = move[3];
      add([=](string &s) {
        auto x1 = s.find(c1);
        auto x2 = s.find(c2);
        swap(s[x1], s[x2]);
      });
    }
  }
}

string dance(string line) {
  for (auto const &s : dance_steps)
    s(line);
  return line;
}

void part1() {
  read();
  cout << dance(start) << '\n';
}

// Because each dance step is invertible, the whole dance is
// invertible, and so the mapping between steps is 1-1 and onto.
// Hence waiting for the starting state to reappear suffices for
// determining cycle length.  (A non-repeated prefix would imply a
// non-invertible mapping where prefix first joins the cycle.)
void part2() {
  read();
  string line = start;
  unsigned remaining = 1000000000;
  for (unsigned i = 0; remaining; ++i) {
    line = dance(line);
    --remaining;
    if (line == start) {
      unsigned cycle_length = i + 1;
      remaining %= cycle_length;
    }
  }
  cout << line << '\n';
}

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
