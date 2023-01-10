// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// Is there some clever way to do this part?  Nothing occurred to me,
// I'll so just brute force it.  I'll keep the buffer so that the
// current position is at the end.  Then inserting after the current
// position and advancing one so that the just-inserted element
// becomes current is just tacking on to the end of the sequence, and
// the answer will always be the element at the start.
void part1() {
  unsigned stride;
  cin >> stride;
  vector<unsigned> buf{ 0 };
  for (int step = 0; step < 2017; ++step) {
    unsigned shift = stride % buf.size();
    // Advancing forward is rotating the buffer to the left, since the
    // current position is always kept at the end
    rotate(buf.begin(), buf.begin() + shift, buf.end());
    buf.push_back(step + 1);
  }
  cout << buf.front() << '\n';
}

// This one has the subtle difference that the answer usually won't
// change.  Conceptually I just need to simulate the above procedure,
// keeping track of where the 0 is.  When the simulation indicates
// that the 0 is at the end (i.e., is current) just before insertion,
// then the insertion is going to happen after the 0 and the
// newly-inserted number becomes the new answer.
void part2() {
  unsigned stride;
  cin >> stride;
  unsigned zero_pos = 0;
  unsigned after_zero = 0;
  unsigned buf_size = 1;
  for (int step = 0; step < 50000000; ++step) {
    unsigned shift = stride % buf_size;
    // Rotate left so that the current position is shifted to the end
    // of the buffer.  The 0's index is currently zero_pos.  It's
    // going to shift to zero_pos - shift, wrapping around if the
    // shift would carry it beyond index 0.
    zero_pos = (zero_pos >= shift ? 0 : buf_size) + zero_pos - shift;
    // Now insert step + 1 at the back of the buffer
    if (zero_pos + 1 == buf_size)
      // Insertion just after 0
      after_zero = step + 1;
    ++buf_size;
  }
  cout << after_zero << '\n';
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
