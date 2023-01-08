// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <optional>
#include <cassert>

using namespace std;

struct program {
  // This program
  string name;
  // How much it weighs
  int weight;
  // Name of programs that it's holding
  vector<string> holding;
  // Cached value for how much the held programs weigh
  optional<int> held_weight;

  program(string const &s);

  // How much does the whole tower rooted here weigh?
  int total_weight();

  // Find the (single) incorrect program within the tower rooted here
  // and return what its weight should be to make everything balanced
  int unbalanced(int wanted_weight);
};

map<string, program> programs;
set<string> being_held;

program &find(string const &name) {
  auto p = programs.find(name);
  assert(p != programs.end());
  return p->second;
}

string root() {
  assert(programs.size() == being_held.size() + 1);
  string result;
  for (auto const &i : programs)
    if (!being_held.count(i.second.name))
      result = i.second.name;
  return result;
}

program::program(string const &s) {
  stringstream ss(s);
  char lpar, rpar;
  ss >> name >> lpar >> weight >> rpar;
  string arrow;
  if (ss >> arrow) {
    assert(arrow == "->");
    string other;
    while (ss >> other) {
      if (other.back() == ',')
        other.pop_back();
      holding.push_back(other);
      being_held.insert(other);
    }
  }
  programs.emplace(name, *this);
}

int program::total_weight() {
  if (!held_weight) {
    held_weight = 0;
    for (auto const &i : holding)
      *held_weight += find(i).total_weight();
  }
  return weight + *held_weight;
}

int program::unbalanced(int wanted_weight) {
  if (holding.empty())
    // The issue must be this leaf's fault
    return wanted_weight;
  optional<int> common_weight;
  for (size_t i = 0; i < holding.size(); ++i)
    for (size_t j = i + 1; j < holding.size(); ++j)
      if (find(holding[i]).total_weight() == find(holding[j]).total_weight())
        common_weight = find(holding[i]).total_weight();
  assert(common_weight);
  optional<int> odd_one;
  for (size_t i = 0; i < holding.size(); ++i)
    if (find(holding[i]).total_weight() != *common_weight) {
      assert(!odd_one);
      odd_one = i;
    }
  if (!odd_one)
    // All things held match, the issue is this node
    return wanted_weight - (total_weight() - weight);
  // The problem is somewhere in the the odd subtower
  return find(holding[*odd_one]).unbalanced(*common_weight);
}

void read() {
  string line;
  while (getline(cin, line))
    program prog(line);
}

void part1() {
  read();
  cout << root() << '\n';
}

void part2() {
  read();
  cout << find(root()).unbalanced(0) << '\n';
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
