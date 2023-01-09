// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct links {
  vector<unsigned> groups;

  unsigned num_programs() const { return groups.size(); }

  unsigned find(unsigned n);
  void connect(unsigned n1, unsigned n2);
};

unsigned links::find(unsigned n) {
  while (n >= groups.size())
    groups.push_back(groups.size());
  unsigned rep = groups[n];
  while (rep != groups[rep])
    rep = groups[rep];
  groups[n] = rep;
  return rep;
}

void links::connect(unsigned n1, unsigned n2) {
  n1 = find(n1);
  n2 = find(n2);
  if (n1 != n2)
    groups[max(n1, n2)] = groups[min(n1, n2)];
}

links find_connections() {
  links lnks;
  string line;
  while (getline(cin, line)) {
    line.push_back(',');
    stringstream ss(line);
    unsigned program;
    string arrow;
    ss >> program >> arrow;
    unsigned other;
    char comma;
    while (ss >> other >> comma)
      lnks.connect(program, other);
  }
  return lnks;
}

void part1() {
  auto lnks = find_connections();
  unsigned group0 = lnks.find(0);
  unsigned group0_size = 0;
  for (unsigned prog = 0; prog < lnks.num_programs(); ++prog)
    if (lnks.find(prog) == group0)
      ++group0_size;
  cout << group0_size << '\n';
}

void part2() {
  auto lnks = find_connections();
  unsigned num_groups = 0;
  for (unsigned prog = 0; prog < lnks.num_programs(); ++prog)
    if (lnks.find(prog) == prog)
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
