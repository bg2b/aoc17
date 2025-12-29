// -*- C++ -*-
// Optimization maybe useful though not critical for part 2
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <unordered_map>
#include <cassert>

using namespace std;

using coords = pair<int, int>;

coords operator+(coords const &c1, coords const &c2) {
  return {c1.first + c2.first, c1.second + c2.second};
}

// This one beats on the state enough that it's worthwhile using a
// hash table instead of the usual ordered map, much as it pains me to
// have to write a hash function for pair<>
struct hash_coords {
  size_t operator()(coords const &xy) const {
    return 100000 * xy.first + xy.second;
  }
};

coords left(coords const &dir) { return {-dir.second, dir.first}; }
coords right(coords const &dir) { return {dir.second, -dir.first}; }
coords reverse(coords const &dir) { return {-dir.first, -dir.second}; }

enum status { clean = 0, weakened, infected, flagged, nstatus };

status toggle(status s) { return s == clean ? infected : clean; }
status cycle(status s) { return status((s + 1) % nstatus); }

struct grid {
  // The carrier's position
  coords carrier{0, 0};
  // The direction the carrier is moving
  coords dir{0, +1};
  // Nodes whose state has been set (everything else is clean)
  unordered_map<coords, status, hash_coords> state;
  // How many infections were caused?
  unsigned infections_caused{0};

  // Contruct from stdin
  grid();

  void set_status(coords const &xy, status s);
  status at(coords const &xy) const;

  // Do one burst, update carrier
  void burst(bool evolved);
};

grid::grid() {
  string row;
  cin >> row;
  size_t n = row.length();
  assert(n % 2 == 1);
  int offset = int(n / 2);
  int y = offset;
  do {
    assert(row.length() == n);
    int x = -offset;
    for (char c : row) {
      if (c == '#')
        state.emplace(make_pair(x, y), infected);
      ++x;
    }
    --y;
  } while (cin >> row);
}

void grid::set_status(coords const &xy, status s) {
  state[xy] = s;
  if (s == infected)
    ++infections_caused;
}

status grid::at(coords const &xy) const {
  auto p = state.find(xy);
  if (p == state.end())
    return clean;
  return p->second;
}

void grid::burst(bool evolved) {
  status s = at(carrier);
  set_status(carrier, evolved ? cycle(s) : toggle(s));
  switch (s) {
  case clean:
    dir = left(dir);
    break;
  case infected:
    dir = right(dir);
    break;
  case flagged:
    dir = reverse(dir);
    break;
  default:
    assert(s == weakened);
    break;
  }
  carrier = carrier + dir;
}

void solve(int iters, bool evolved) {
  grid grd;
  for (int _ = 0; _ < iters; ++_)
    grd.burst(evolved);
  cout << grd.infections_caused << " infections caused\n";
}

void part1() { solve(10000, false); }
void part2() { solve(10000000, true); }

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
