// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

// I'll identify hexes with combos of two basis vectors.  Having one
// for n seems natural as (1, 0), and I'll arbitrarily choose ne as
// the second (0, 1).  Moves n or ne or s or sw are then trivial
// adding or subtracting 1 from one of the components.  Moves nw and
// se are (1, -1) and (-1, 1) respectively.
/*
        +--+
       /    \
   +--+ 1, 0 +--+
  /    \    /    \
 + 1,-1 +--+ 0, 1 +
  \    /    \    /
   +--+ 0, 0 +--+
  /    \    /    \
 + 0,-1 +--+ -1,1 +
  \    /    \    /
   +--+ -1,0 +--+
       \    /
        +--+
*/

using coord = pair<int, int>;

coord operator+(coord const &c1, coord const &c2) {
  return {c1.first + c2.first, c1.second + c2.second};
}

map<string, coord> dirs{{"n", {+1, 0}}, {"ne", {0, +1}}, {"se", {-1, +1}},
                        {"s", {-1, 0}}, {"sw", {0, -1}}, {"nw", {+1, -1}}};

// For distance (number of steps), if a point is in the first or third
// "quadrants" (n and ne coefficients have the same sign), then it's
// trivial (basically like Manhattan distance).  For other quadrants,
// use the "diagonal" moves.
int distance(coord const &c) {
  auto [n, ne] = c;
  if (n * ne < 0)
    // This is in a quadrant where a move se or nw is useful.  One of
    // those can be used min(abs(n), abs(ne)) times to reduce the
    // smaller magnitude coordinate to 0, i.e.,
    //   int diag = min(abs(n), abs(ne));
    // Whatever's left can be gone to directly by n, ne, s, or sw.
    //   int straight = max(abs(n), abs(ne)) - diag;
    // Total distance is the sum of those, and diag nicely cancels,
    // leaving just...
    return max(abs(n), abs(ne));
  else
    // Simple "Manhattan" case
    return abs(n) + abs(ne);
}

pair<int, int> child_meanderings() {
  string steps;
  cin >> steps;
  steps.push_back(',');
  coord child{0, 0};
  int max_dist = 0;
  for (size_t pos = 0; pos < steps.length();) {
    size_t next_pos = steps.find(',', pos);
    string dir = steps.substr(pos, next_pos - pos);
    auto p = dirs.find(dir);
    assert(p != dirs.end());
    child = child + p->second;
    max_dist = max(max_dist, distance(child));
    pos = next_pos + 1;
  }
  return {distance(child), max_dist};
}

void part1() { cout << child_meanderings().first << '\n'; }
void part2() { cout << child_meanderings().second << '\n'; }

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
