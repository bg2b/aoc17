// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cassert>

using namespace std;

using coords = pair<int, int>;

coords operator+(coords const &c1, coords const &c2) {
  return {c1.first + c2.first, c1.second + c2.second};
}

coords direction(unsigned index) {
  switch (index % 4) {
  case 0:
    return {-1, 0};
  case 1:
    return {0, +1};
  case 2:
    return {+1, 0};
  default:
    return {0, -1};
  }
}

struct routing {
  vector<string> diagram;
  coords start;

  routing();

  char at(coords const &c) const { return diagram[c.first][c.second]; }

  pair<string, unsigned> follow() const;
};

routing::routing() {
  string row;
  while (getline(cin, row)) {
    assert(row.length() > 2);
    assert(row.front() == ' ' && row.back() == ' ');
    diagram.push_back(row);
    assert(row.length() == diagram.front().length());
  }
  assert(diagram.front().find('|') != string::npos);
  assert(diagram.back() == string(diagram.back().length(), ' '));
  start = {0, diagram.front().find('|')};
}

pair<string, unsigned> routing::follow() const {
  coords pos = start;
  unsigned dir = 2;
  string passed;
  unsigned steps = 1;
  while (true) {
    if (isalpha(at(pos)))
      passed.push_back(at(pos));
    if (at(pos + direction(dir)) == ' ') {
      if (at(pos + direction(dir + 1)) != ' ')
        dir += 1;
      else if (at(pos + direction(dir + 3)) != ' ')
        dir += 3;
      else
        break;
    }
    pos = pos + direction(dir);
    ++steps;
  }
  return {passed, steps};
}

void part1() { cout << routing().follow().first << '\n'; }
void part2() { cout << routing().follow().second << '\n'; }

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
