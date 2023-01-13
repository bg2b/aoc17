// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <cassert>

using namespace std;

// Indexed by whatever the head is reading
struct state {
  array<int, 2> write;
  array<int, 2> dir;
  array<int, 2> next;
};

struct turing_machine {
  // Current state
  int current;
  // Number of steps before checksum
  int steps;
  // State table
  vector<state> states;
  // Tape contents (outside of this is all 0)
  list<int> tape;
  // Where the head is
  list<int>::iterator head;

  // Construct from stdin
  turing_machine();

  // Move head left or right, extending tape if needed
  void left();
  void right();
  // Execute one step, return true until it's time for the checksum
  bool step();
  // Compute tape checksum
  int checksum() const;
};

turing_machine::turing_machine() {
  auto get = [](string const &what) {
               string line;
               getline(cin, line);
               auto pos = line.find(what);
               assert(pos != string::npos);
               return line.substr(pos + what.length() + 1);
             };
  current = get("state")[0] - 'A';
  steps = stoi(get("after"));
  string blank;
  while (getline(cin, blank)) {
    assert(blank.empty());
    unsigned s = get("state")[0] - 'A';
    assert(s == states.size());
    state st;
    for (int current = 0; current <= 1; ++current) {
      int c = stoi(get("is"));
      assert(c == current);
      st.write[current] = stoi(get("value"));
      st.dir[current] = get("the")[0] == 'r' ? +1 : -1;
      st.next[current] = get("state")[0] - 'A';
    }
    states.push_back(st);
  }
  tape.push_back(0);
  head = tape.begin();
}

void turing_machine::left() {
  if (head == tape.begin())
    tape.push_front(0);
  --head;
}

void turing_machine::right() {
  if (head == prev(tape.end()))
    tape.push_back(0);
  ++head;
}

bool turing_machine::step() {
  int reading = *head;
  auto const &st = states[current];
  *head = st.write[reading];
  if (st.dir[reading] < 0)
    left();
  else
    right();
  current = st.next[reading];
  return --steps > 0;
}

int turing_machine::checksum() const {
  int result = 0;
  for (auto i : tape)
    result += i;
  return result;
}

void part1() {
  turing_machine tm;
  while (tm.step())
    ;
  cout << tm.checksum() << '\n';
}

void part2() {
  cout << "Reboot the Printer!\n";
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
