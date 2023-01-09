// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct firewall {
  vector<pair<unsigned, unsigned>> layers;

  firewall();

  unsigned severity(unsigned t0) const;
  bool caught(unsigned t0) const;
};

firewall::firewall() {
  unsigned depth, range;
  char colon;
  while (cin >> depth >> colon >> range)
    layers.emplace_back(depth, range);
}

unsigned firewall::severity(unsigned t0) const {
  unsigned total = 0;
  for (auto [depth, range] : layers) {
    // t = when the packet moves into the layer with that depth
    unsigned t = t0 + depth;
    // The scanner period is 2 * (range - 1), starting from time 0
    if (t % (2 * range - 2) == 0)
      total += depth * range;
  }
  return total;
}

bool firewall::caught(unsigned t0) const {
  for (auto [depth, range] : layers) {
    unsigned t = t0 + depth;
    if (t % (2 * range - 2) == 0)
      return true;
  }
  return false;
}

void part1() { cout << firewall().severity(0) << '\n'; }

void part2() {
  firewall fw;
  unsigned t0;
  for (t0 = 0; fw.caught(t0); ++t0)
    ;
  cout << t0 << '\n';
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
