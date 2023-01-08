// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <map>
#include <cassert>

using namespace std;

pair<int, int> to_grid(int index) {
  if (index == 1)
    return { 0, 0 };
  // Consecutive rings end with odd squares.  Counting rings starting
  // from 0, find the ring for the index
  auto ring_end = [](int ring) { return (2 * ring + 1) * (2 * ring + 1); };
  int ring = 0;
  while (index > ring_end(ring))
    ++ring;
  assert(ring > 0);
  // Get the starting and ending indexes on the ring, and the length
  // of one of the sides.  The first side starts with the lowest index
  // on the ring, which is just to the right of the corner where the
  // previous ring ends.
  int start = ring_end(ring - 1) + 1;
  int end = ring_end(ring);
  int side = (end - start + 1) / 4;
  // Go around the ring and see which side the index is on
  index -= start;
  int side_num = 0;
  while (index >= side) {
    index -= side;
    ++side_num;
  }
  // Shift so that the the middle of the side is at 0
  index -= side / 2 - 1;
  // Rotate to get the final x, y
  auto coords = make_pair(ring, index);
  while (side_num-- > 0)
    coords = make_pair(-coords.second, coords.first);
  return coords;
}

void part1() {
  int loc;
  cin >> loc;
  auto [x, y] = to_grid(loc);
  cout << abs(x) + abs(y) << '\n';
}

void part2() {
  int threshold;
  cin >> threshold;
  map<pair<int, int>, int> sums;
  sums[{ 0, 0 }] = 1;
  int index = 1;
  while (sums[to_grid(index)] <= threshold) {
    ++index;
    auto [x, y] = to_grid(index);
    int sum_adj = 0;
    for (int dx = -1; dx <= +1; ++dx)
      for (int dy = -1; dy <= +1; ++dy)
        sum_adj += sums[{ x + dx, y + dy }];
    sums[{ x, y }] = sum_adj;
  }
  cout << sums[to_grid(index)] << '\n';
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
