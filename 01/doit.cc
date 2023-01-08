// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

void solve(bool halfway) {
  string digits;
  cin >> digits;
  int ans = 0;
  size_t n = digits.length();
  assert(!halfway || n % 2 == 0);
  size_t offset = halfway ? n / 2 : 1;
  for (size_t i = 0; i < n; ++i)
    if (digits[i] == digits[(i + offset) % n])
      ans += digits[i] - '0';
  cout << ans << '\n';
}

void part1() { solve(false); }
void part2() { solve(true); }

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
