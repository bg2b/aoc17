// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <cassert>

using namespace std;

// The available components
vector<pair<int, int>> components;
// Map from the end of the bridge to the indexes of the components
// that can be used to extend the bridge
map<int, vector<int>> extending;

void read() {
  int e1, e2;
  char slash;
  while (cin >> e1 >> slash >> e2) {
    int index = components.size();
    components.emplace_back(e1, e2);
    extending[e1].push_back(index);
    if (e1 != e2)
      extending[e2].push_back(index);
  }
}

// The bridge is just a vector ports, with the back being extended.
// For fast checking of available extensions, used[i] has nonzero
// values when component i has been used in the bridge so far.
void search(vector<int> &bridge, vector<char> &used,
            function<void(int strength, size_t len)> check) {
  int end = bridge.back();
  bool any_extensions = false;
  auto p = extending.find(end);
  if (p != extending.end())
    // Look through potential component indexes for extension
    for (auto i : p->second)
      if (!used[i]) {
        // This one hasn't been used yet
        any_extensions = true;
        auto const &c = components[i];
        used[i] = 1;
        // Extend the bridge
        bridge.push_back(c.first == end ? c.second : c.first);
        search(bridge, used, check);
        // Reset
        bridge.pop_back();
        used[i] = 0;
      }
  if (!any_extensions) {
    // The bridge is as long as it can get
    int strength = 0;
    for (size_t i = 0; i + 1 < bridge.size(); ++i)
      strength += bridge[i] + bridge[i + 1];
    check(strength, bridge.size());
  }
}

void solve(function<void(int strength, size_t len)> check) {
  read();
  vector<int> bridge{ 0 };
  vector<char> used(components.size(), 0);
  search(bridge, used, check);
}

void part1() {
  int strongest = 0;
  solve([&](int strength, size_t) { strongest = max(strongest, strength); });
  cout << strongest << '\n';
}

void part2() {
  int strongest = 0;
  size_t longest = 0;
  solve([&](int strength, size_t length) {
          if (length > longest) {
            longest = length;
            strongest = strength;
          } else if (length == longest)
            strongest = max(strongest, strength);
        });
  cout << strongest << '\n';
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
