// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

bool is_square(vector<string> const &ss) {
  return !ss.empty() && ss.size() == ss.front().size();
}

vector<string> start = { ".#.",
                         "..#",
                         "###" };

size_t count_pixels(vector<string> const &ss) {
  size_t result = 0;
  for (auto const &s : ss)
    result += count(s.begin(), s.end(), '#');
  return result;
}

// .#./..#/### => start
vector<string> unpack(string const &s) {
  vector<string> result;
  size_t pos = 0;
  while (true) {
    auto next_pos = s.find('/', pos);
    result.push_back(s.substr(pos, next_pos - pos));
    if (next_pos == string::npos)
      break;
    pos = next_pos + 1;
  }
  assert(is_square(result));
  return result;
}

// start => .#./..#/###
string pack(vector<string> const &ss) {
  assert(is_square(ss));
  string result;
  for (auto const &s : ss) {
    if (!result.empty())
      result.push_back('/');
    result += s;
  }
  return result;
}

// Rotate unpacked form 90 degrees
vector<string> rotate(vector<string> &ss) {
  assert(is_square(ss));
  size_t n = ss.size();
  auto result = ss;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      result[j][n - 1 - i] = ss[i][j];
  return result;
}

// Flip unpacked form along the column dimension
vector<string> flip(vector<string> const &ss) {
  assert(is_square(ss));
  size_t n = ss.size();
  auto result = ss;
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      result[i][n - 1 - j] = ss[i][j];
  return result;
}

// Convert the (unpacked) pattern into a linear array of packed forms,
// each for one small subsquare of the pattern
vector<string> split(vector<string> const &pattern) {
  assert(is_square(pattern));
  size_t n = pattern.size();
  size_t small;
  if (n % 2 == 0)
    small = 2;
  else {
    assert(n % 3 == 0);
    small = 3;
  }
  size_t m = n / small;
  vector<string> result;
  for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < m; ++j) {
      string s = pattern[i * small].substr(j * small, small);
      for (size_t k = 1; k < small; ++k)
        s += '/' + pattern[i * small + k].substr(j * small, small);
      result.push_back(s);
    }
  return result;
}

// Concatenate a linear array of packed forms back into a pattern
// (unpacked)
vector<string> concat(vector<string> const &ss) {
  size_t m;
  for (m = 1; m * m < ss.size(); ++m)
    ;
  assert(m * m == ss.size());
  size_t small = unpack(ss.front()).size();
  vector<string> result(m * small);
  for (size_t i = 0; i < m; ++i)
    for (size_t j = 0; j < m; ++j) {
      auto grid = unpack(ss[i * m + j]);
      for (size_t k = 0; k < small; ++k)
        result[i * small + k] += grid[k];
    }
  return result;
}

struct enhancements {
  // Table of rules, including all rotations and reflections
  map<string, string> rules;

  // Construct from stdin
  enhancements();

  // Do one step of pattern enhancement
  vector<string> enhance(vector<string> const &pattern);
};

enhancements::enhancements() {
  string from, arrow, to;
  while (cin >> from >> arrow >> to) {
    auto unpacked = unpack(from);
    for (int _ = 0; _ < 4; ++_) {
      rules[pack(unpacked)] = to;
      unpacked = rotate(unpacked);
    }
    unpacked = flip(unpacked);
    for (int _ = 0; _ < 4; ++_) {
      rules[pack(unpacked)] = to;
      unpacked = rotate(unpacked);
    }
  }
}

vector<string> enhancements::enhance(vector<string> const &pattern) {
  auto parts = split(pattern);
  for (auto &part : parts) {
    auto p = rules.find(part);
    assert(p != rules.end());
    part = p->second;
  }
  return concat(parts);
}

void solve(int iterations) {
  enhancements enh;
  auto pattern = start;
  for (int _ = 0; _ < iterations; ++_)
    pattern = enh.enhance(pattern);
  cout << count_pixels(pattern) << '\n';
}

void part1() { solve(5); }
void part2() { solve(18); }

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
