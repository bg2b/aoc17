// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>

using namespace std;

int const N = 256;

using list = array<int, N>;

// To avoid having to deal with wrapping in operations, it's good if
// the current position is always at the start of the (implicitly
// circular) list.  The only wrinkle is that when doing the final
// processing, the list has to be rotated back to the original spot.
struct hasher {
  // Hashing sequence
  vector<int> lengths;
  // Added for the next skip
  int skip_size{0};
  // Total amount of skipping so far
  int total_skip{0};
  // The list with current position at index 0
  list l;

  hasher(vector<int> const &lengths_);

  // Do one round of the hash
  void round();
  // Finish by rotating so the original head is back at index 0
  void finish();

  int product() const { return l[0] * l[1]; }
  // As described in part 2
  string dense_hash() const;
};

hasher::hasher(vector<int> const &lengths_) : lengths(lengths_) {
  for (int i = 0; i < N; ++i)
    l[i] = i;
}

void hasher::round() {
  for (auto length : lengths) {
    reverse(l.begin(), l.begin() + length);
    int skip = length + skip_size;
    total_skip += skip;
    skip %= N;
    // Rotate left to put the start of the next reversal at index 0
    rotate(l.begin(), l.begin() + skip, l.end());
    ++skip_size;
  }
}

void hasher::finish() {
  // Rotate right to undo all the previous left rotations
  total_skip %= N;
  rotate(l.rbegin(), l.rbegin() + total_skip, l.rend());
}

string hasher::dense_hash() const {
  assert(N == 16 * 16);
  string hash;
  auto hex = [](int c) { return c > 9 ? c - 10 + 'a' : c + '0'; };
  for (size_t i = 0; i < 16; ++i) {
    int byte = 0;
    for (size_t j = 0; j < 16; ++j)
      byte ^= l[16 * i + j];
    assert(byte < 256);
    hash.push_back(hex(byte >> 4));
    hash.push_back(hex(byte & 0xf));
  }
  return hash;
}

void part1() {
  int length;
  cin >> length;
  vector<int> lengths(1, length);
  char comma;
  while (cin >> comma >> length)
    lengths.push_back(length);
  hasher h(lengths);
  h.round();
  h.finish();
  cout << h.product() << '\n';
}

void part2() {
  string bytes;
  cin >> bytes;
  vector<int> lengths;
  for (auto c : bytes)
    lengths.push_back(c);
  for (int extra : { 17, 31, 73, 47, 23 })
    lengths.push_back(extra);
  hasher h(lengths);
  for (int _ = 0; _ < 64; ++_)
    h.round();
  h.finish();
  cout << h.dense_hash() << '\n';
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
