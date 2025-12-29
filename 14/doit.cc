// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
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

  // Expanded bit form
  string bit_hash() const;
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

string hasher::bit_hash() const {
  assert(N == 16 * 16);
  string hash;
  for (size_t i = 0; i < 16; ++i) {
    int byte = 0;
    for (size_t j = 0; j < 16; ++j)
      byte ^= l[16 * i + j];
    assert(byte < 256);
    for (int i = 0; i < 8; ++i)
      hash.push_back((byte & (1 << (7 - i))) ? '@' : '.');
  }
  return hash;
}

string hashed(string const &bytes) {
  vector<int> lengths;
  for (auto c : bytes)
    lengths.push_back(c);
  for (int extra : {17, 31, 73, 47, 23})
    lengths.push_back(extra);
  hasher h(lengths);
  for (int _ = 0; _ < 64; ++_)
    h.round();
  h.finish();
  return h.bit_hash();
}

unsigned const grid_size = 128;

// Run a scan over the disk, calling fn for each row with the row
// index and the map of used sectors for that row.
void disk_map(function<void(unsigned row, string const &hsh)> fn) {
  string key;
  cin >> key;
  for (unsigned row = 0; row < grid_size; ++row)
    fn(row, hashed(key + '-' + to_string(row)));
}

void part1() {
  unsigned used = 0;
  disk_map([&](unsigned, string const &hsh) {
    used += count(hsh.begin(), hsh.end(), '@');
  });
  cout << used << '\n';
}

// For part 2, it's possible to use a scan line approach, keeping at
// most two rows around at any time.  Groups would get counted as the
// row scan passes them (basically you're looking for all the columns
// corresponding to a group having nothing in the same columns of the
// next row).  It's a little tricky though, and a brute force
// union-find that keeps everything requires no thought.  Maybe for a
// doit1.cc later...

using sector = pair<unsigned, unsigned>;

struct sectors {
  vector<unsigned> links;

  sectors();

  // Map to 1D for the union-find
  unsigned index(sector const &s) const {
    return grid_size * s.first + s.second;
  }

  // Find the representative for a sector index
  unsigned find(unsigned ndx);
  // Two sectors with the corresponding indexes are adjacent
  void adjacent(unsigned ndx1, unsigned ndx2);
};

sectors::sectors() : links(grid_size * grid_size) {
  for (size_t i = 0; i < links.size(); ++i)
    links[i] = i;
}

unsigned sectors::find(unsigned ndx) {
  unsigned rep = links[ndx];
  while (rep != links[rep])
    rep = links[rep];
  links[ndx] = rep;
  return rep;
}

void sectors::adjacent(unsigned ndx1, unsigned ndx2) {
  ndx1 = find(ndx1);
  ndx2 = find(ndx2);
  if (ndx1 != ndx2)
    links[max(ndx1, ndx2)] = links[min(ndx1, ndx2)];
}

void part2() {
  sectors s;
  string last_hsh;
  vector<unsigned> used;
  // Sweep over the disk
  disk_map([&](unsigned row, string const &hsh) {
    assert(hsh.size() == grid_size);
    for (unsigned col = 0; col < grid_size; ++col)
      if (hsh[col] == '@') {
        // Save each used sector
        auto ndx = s.index({row, col});
        used.push_back(ndx);
        // Link to adjacent used sectors
        if (row > 0 && last_hsh[col] == '@')
          s.adjacent(ndx, s.index({row - 1, col}));
        if (col + 1 < grid_size && hsh[col + 1] == '@')
          s.adjacent(ndx, s.index({row, col + 1}));
      }
    last_hsh = hsh;
  });
  // Count the groups
  unsigned num_groups = 0;
  for (auto ndx : used)
    if (s.find(ndx) == ndx)
      ++num_groups;
  cout << num_groups << '\n';
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
