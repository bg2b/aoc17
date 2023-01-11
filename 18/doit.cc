// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <functional>
#include <cassert>

using namespace std;

struct process;

using num = long;
using instr = function<void(process &player)>;
// register or immediate, first ? register[second] : second
using reg_imm = pair<bool, num>;

struct process {
  // Register file
  array<num, 26> reg;
  // Program counter
  num pc{0};
  // Instructions to be executed
  vector<instr> instructions;
  // Data that's been transmitted and not yet received
  list<num> sent;
  // Total number of sent items
  unsigned num_sends{0};
  // true => blocked waiting to receive data
  bool waiting{false};
  // The other process in a duet
  process *partner{nullptr};

  // Construct from stdin
  process();
  // Pair up with another process; each is the partner of the other
  // one.  other becomes process 0, the new one is process 1.
  process(process &other);

  bool solo() const { return !partner; }

  num get(reg_imm const &val) const {
    return val.first ? reg[val.second] : val.second;
  }
  void set(reg_imm const &arg, num n) {
    assert(arg.first);
    reg[arg.second] = n;
  }

  bool has_data() const { return !sent.empty(); }
  num transmit() {
    assert(has_data());
    num n = sent.front();
    sent.pop_front();
    return n;
  }

  void snd(num n) { ++num_sends; sent.push_back(n); }
  // rcv behavior varies depending on part 1 or 2
  void rcv(reg_imm const &arg);
  // Regular pc incrementing is going to add one, so have to adjust
  // for that
  void jgz(num test, num offset) { if (test > 0) pc += offset - 1; }

  // Try to execute instructions, return true if any progress was
  // made.  false indicates either already terminated or blocked
  // waiting at a rcv.
  bool execute();

  // Last item sent, for part 1
  num part1_ans() const { return sent.back(); }
  // Number of items sent, for part 2
  num part2_ans() const { return num_sends; }
};

process::process() {
  for (size_t i = 0; i < reg.size(); ++i)
    reg[i] = 0;
  auto ri = []() -> reg_imm {
              string arg;
              cin >> arg;
              if (arg.length() == 1 && arg[0] >= 'a' && arg[0] <= 'z')
                return { true, arg[0] - 'a' };
              return { false, stoi(arg) };
            };
  auto add = [&](instr const &i) { instructions.push_back(i); };
  string op;
  while (cin >> op) {
    reg_imm arg1 = ri();
    if (op == "snd")
      add([=](process &player) { player.snd(player.get(arg1)); });
    else if (op == "rcv")
      add([=](process &player) { player.rcv(arg1); });
    else {
      reg_imm arg2 = ri();
      if (op == "set")
        add([=](process &player) {
              player.set(arg1, player.get(arg2));
            });
      else if (op == "add")
        add([=](process &player) {
              player.set(arg1, player.get(arg1) + player.get(arg2));
            });
      else if (op == "mul")
        add([=](process &player) {
              player.set(arg1, player.get(arg1) * player.get(arg2));
            });
      else if (op == "mod")
        add([=](process &player) {
              player.set(arg1, player.get(arg1) % player.get(arg2));
            });
      else {
        assert(op == "jgz");
        add([=](process &player) {
              player.jgz(player.get(arg1), player.get(arg2));
            });
      }
    }
  }
}

process::process(process &other) {
  *this = other;
  reg['p' - 'a'] = 1;
  other.partner = this;
  partner = &other;
}

void process::rcv(reg_imm const &arg) {
  if (solo()) {
    // Part 1
    if (get(arg) == 0)
      return;
    assert(!sent.empty());
    // Force termination
    pc = instructions.size();
  } else {
    waiting = !partner->has_data();
    if (!waiting)
      set(arg, partner->transmit());
  }
}

bool process::execute() {
  bool progress = false;
  while (pc >= 0 && pc < num(instructions.size())) {
    instructions[pc](*this);
    if (waiting)
      // In a duet and tried to receive, but no data was available.
      // Stop and give the partner a chance to run.
      break;
    ++pc;
    progress = true;
  }
  return progress;
}

void part1() {
  process player;
  player.execute();
  cout << player.part1_ans() << '\n';
}

void part2() {
  process player1;
  process player2(player1);
  // Alternate execution until no one can make any progress
  bool progress;
  do {
    progress = false;
    progress = player1.execute() || progress;
    progress = player2.execute() || progress;
  } while (progress);
  cout << player2.part2_ans() << '\n';
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
