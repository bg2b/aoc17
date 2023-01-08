// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <map>
#include <cassert>

using namespace std;

bool compare(string const &cmp, int v1, int v2) {
  if (cmp == "==")    return v1 == v2;
  if (cmp == "!=")    return v1 != v2;
  if (cmp == ">=")    return v1 >= v2;
  if (cmp == "<=")    return v1 <= v2;
  if (cmp == ">")     return v1 > v2;
  assert(cmp == "<"); return v1 < v2;
}

pair<int, int> execute() {
  string reg1, dir, if_, reg2, cmp;
  int val1, val2;
  map<string, int> registers;
  int max_during = 0;
  while (cin >> reg1 >> dir >> val1 >> if_ >> reg2 >> cmp >> val2) {
    if (dir == "dec")
      val1 = -val1;
    if (compare(cmp, registers[reg2], val2)) {
      registers[reg1] += val1;
      max_during = max(max_during, registers[reg1]);
    }
  }
  int max_after = 0;
  for (auto const &i : registers)
    max_after = max(max_after, i.second);
  return { max_after, max_during };
}

void part1() { cout << execute().first << '\n'; }
void part2() { cout << execute().second << '\n'; }

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
