// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <cassert>

using namespace std;

using num = unsigned long;

num const mod = 2147483647;

struct generator {
  num scale;
  num state;
  num mask;

  generator(num scale_, num seed, num mask_)
      : scale(scale_), state(seed), mask(mask_) {}

  num next() {
    do
      state = (scale * state) % mod;
    while (state & mask);
    return state;
  }
};

void judge(int num_pairs, num maskA, num maskB) {
  string gen, _, starts, with;
  size_t a, b;
  cin >> gen >> _ >> starts >> with >> a;
  cin >> gen >> _ >> starts >> with >> b;
  generator A(16807, a, maskA);
  generator B(48271, b, maskB);
  unsigned matches = 0;
  for (int _ = 0; _ < num_pairs; ++_)
    if ((A.next() & 0xffff) == (B.next() & 0xffff))
      ++matches;
  cout << matches << '\n';
}

void part1() { judge(40000000, 0, 0); }
void part2() { judge(5000000, 0x3, 0x7); }

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
