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

struct coprocessor;

using num = long;
using instr = function<void(coprocessor &copr)>;
// register or immediate, first ? register[second] : second
using reg_imm = pair<bool, num>;

struct coprocessor {
  // Register file
  array<num, 8> reg;
  // Program counter
  num pc{0};
  // Instructions to be executed
  vector<instr> instructions;
  unsigned num_muls{0};

  // Construct from stdin
  coprocessor();

  num get(reg_imm const &val) const {
    return val.first ? reg[val.second] : val.second;
  }
  void set(reg_imm const &arg, num n) {
    assert(arg.first);
    reg[arg.second] = n;
  }

  // Regular pc incrementing is going to add one, so have to adjust
  // for that
  void jnz(num test, num offset) {
    if (test != 0)
      pc += offset - 1;
  }

  void execute();
};

coprocessor::coprocessor() {
  for (size_t i = 0; i < reg.size(); ++i)
    reg[i] = 0;
  auto ri = []() -> reg_imm {
    string arg;
    cin >> arg;
    if (arg.length() == 1 && arg[0] >= 'a' && arg[0] <= 'h')
      return {true, arg[0] - 'a'};
    return {false, stoi(arg)};
  };
  auto add = [&](instr const &i) { instructions.push_back(i); };
  string op;
  while (cin >> op) {
    reg_imm arg1 = ri();
    reg_imm arg2 = ri();
    if (op == "set")
      add([=](coprocessor &copr) { copr.set(arg1, copr.get(arg2)); });
    else if (op == "sub")
      add([=](coprocessor &copr) {
        copr.set(arg1, copr.get(arg1) - copr.get(arg2));
      });
    else if (op == "mul")
      add([=](coprocessor &copr) {
        ++copr.num_muls;
        copr.set(arg1, copr.get(arg1) * copr.get(arg2));
      });
    else {
      assert(op == "jnz");
      add([=](coprocessor &copr) { copr.jnz(copr.get(arg1), copr.get(arg2)); });
    }
  }
}

void coprocessor::execute() {
  while (pc >= 0 && pc < num(instructions.size())) {
    instructions[pc](*this);
    ++pc;
  }
}

void part1() {
  coprocessor copr;
  copr.execute();
  cout << copr.num_muls << '\n';
}

// Transcribed and replaced the inner loops involving d, e
void part2() {
  long b = 0, c = 0, d = 0, f = 0, g = 0, h = 0;
  // set b 93
  b = 93;
  // set c b
  c = b;
  // mul b 100
  b *= 100;
  // sub b -100000
  b += 100000;
  // set c b
  c = b;
  // sub c -17000
  c += 17000;
L1:
  // set f 1
  f = 1;
  // check if b is prime
  for (d = 2; d * d <= b; ++d)
    if (b % d == 0) {
      f = 0;
      break;
    }
  if (f == 0)
    ++h;
  // set g b
  g = b;
  // sub g c
  g -= c;
  // jnz g 2
  if (g != 0) {
    b += 17;
    goto L1;
  }
  cout << h << '\n';
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
