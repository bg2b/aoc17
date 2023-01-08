// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

pair<int, int> process(string const &s) {
  int total_score = 0;
  int garbage_removed = 0;
  int active_groups = 0;
  bool in_garbage = false;
  size_t i = 0;
  while (i < s.length()) {
    if (!in_garbage && s[i] == '<') {
      in_garbage = true;
      ++i;
      continue;
    }
    if (in_garbage) {
      if (s[i] == '>')
        in_garbage = false;
      else if (s[i] == '!') {
        assert(i + 1 < s.length());
        ++i;  // plus one more below
      } else
        ++garbage_removed;
      ++i;
      continue;
    }
    assert(!in_garbage);
    if (s[i] == '{') {
      ++active_groups;
      total_score += active_groups;
    } else if (s[i] == '}') {
      assert(active_groups > 0);
      --active_groups;
    }
    ++i;
  }
  assert(active_groups == 0);
  return { total_score, garbage_removed };
}

void part1() {
  string line;
  while (getline(cin, line))
    cout << process(line).first << '\n';
}

void part2() {
  string line;
  while (getline(cin, line))
    cout << process(line).second << '\n';
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
