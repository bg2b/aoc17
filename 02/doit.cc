// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

using namespace std;

void solve(function<int(vector<int> &row)> fn) {
  int ans = 0;
  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    vector<int> row;
    int n;
    while (ss >> n)
      row.push_back(n);
    assert(!row.empty());
    ans += fn(row);
  }
  cout << ans << '\n';
}

void part1() {
  solve([](vector<int> &row) {
    auto [min, max] = minmax_element(row.begin(), row.end());
    return *max - *min;
  });
}

void part2() {
  solve([](vector<int> &row) {
    sort(row.begin(), row.end());
    assert(row.front() > 0);
    for (size_t i = 0; i < row.size(); ++i)
      for (size_t j = i + 1; j < row.size(); ++j)
        if (row[j] % row[i] == 0)
          return row[j] / row[i];
    return 0;
  });
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
