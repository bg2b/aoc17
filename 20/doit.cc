// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

using vec = array<int, 3>;
using particle = array<vec, 3>;

vector<particle> read() {
  auto getvec = [](string const &s, char v) {
                  auto start = s.find(v);
                  assert(start != string::npos && start + 8 < s.length());
                  assert(s[start + 1] == '=' && s[start + 2] == '<');
                  auto end = s.find('>', start + 1);
                  assert(end != string::npos);
                  string xyz = s.substr(start + 3, end - start + 1 - 4);
                  char comma;
                  stringstream ss(xyz);
                  vec result;
                  ss >> result[0] >> comma >> result[1] >> comma >> result[2];
                  return result;
                };
  vector<particle> result;
  string line;
  while (getline(cin, line)) {
    particle p;
    p[0] = getvec(line, 'p');
    p[1] = getvec(line, 'v');
    p[2] = getvec(line, 'a');
    result.push_back(p);
  }
  return result;
}

void part1() {
  auto particles = read();
  auto slowest =
    min_element(particles.begin(), particles.end(),
                [](particle const &p1, particle const &p2) {
                  // Only acceleration matters in the end
                  auto const &a1 = p1[2];
                  auto const &a2 = p2[2];
                  int mag1 = abs(a1[0]) + abs(a1[1]) + abs(a1[2]);
                  int mag2 = abs(a2[0]) + abs(a2[1]) + abs(a2[2]);
                  return mag1 < mag2;
                });
  cout << slowest - particles.begin() << '\n';
}

// Considering how acceleration works (adds to velocity, which then
// addes to position in the same timestep), the equation for the
// position of one particle turns out to be:
//    p_t = p + t*v + 0.5*t*(t+1)*a
// Expanding:
//    p_t = p + (v+0.5*a) * t + (0.5*a) * t^2

// All collision times in 1D for two particles with the given
// position, velocity, acceleration.  Because motions are quadratic,
// there can be at most two, except the special { -1 } indicates all
// times (consider, e.g., two particles in 3D but with exactly the
// same x position, velocity, acceleration).
//
// To keep everything in integer arithmetic, the above equations for
// the two particles can be scaled by 2; collision times are not
// affected by this.
vector<int> collision(int p1, int v1, int a1, int p2, int v2, int a2) {
  // Express the distance between the particles as a quadratic.  The
  // a, b, c below are as in the quadratic formula, and remember the
  // scaling by 2.
  int a = a2 - a1;
  int b = 2 * (v2 - v1) + a;
  int c = 2 * (p2 - p1);
  if (a == 0 && b == 0 && c == 0)
    // Colliding at all times
    return { -1 };
  if (a == 0 && b == 0)
    // Never colliding
    return {};
  if (a == 0) {
    // Linear case, need integer t with b * t + c = 0
    int t = -c / b;
    if (t < 0 || b * t + c != 0)
      return {};
    return { t };
  }
  // General quadratic
  int discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    // No real solutions
    return {};
  int sqrt_disc = int(sqrt(discriminant));
  if (discriminant != sqrt_disc * sqrt_disc)
    // Not going to be integer
    return {};
  vector<int> result;
  auto maybe = [&](int t) {
                 if (t >= 0 && a * t * t + b * t + c == 0)
                   result.push_back(t);
               };
  maybe((-b + sqrt_disc) / (2 * a));
  if (sqrt_disc != 0)
    // No need for repeated roots
    maybe((-b - sqrt_disc) / (2 * a));
  // Ensure sorted order
  if (result.size() == 2 && result[0] > result[1])
    swap(result[0], result[1]);
  return result;
}

// First collision time (if any) between two given particles
optional<int> collision(particle const &p1, particle const &p2) {
  vector<int> all_ts;
  for (int i = 0; i < 3; ++i) {
    auto ts = collision(p1[0][i], p1[1][i], p1[2][i],
                        p2[0][i], p2[1][i], p2[2][i]);
    if (ts.empty())
      // No collision in this dimension
      return nullopt;
    if (ts == vector<int>{ -1 })
      // No constraint in this dimension
      continue;
    if (all_ts.empty()) {
      // This dimension imposes the first constraint
      all_ts = ts;
      continue;
    }
    // Already some constraints from previous dimensions
    vector<int> next_all_ts;
    // Note that results from the 1D collision() are sorted
    set_intersection(all_ts.begin(), all_ts.end(),
                     ts.begin(), ts.end(),
                     back_inserter(next_all_ts));
    all_ts = next_all_ts;
  }
  if (all_ts.empty())
    return nullopt;
  return all_ts.front();
}

vec operator+(vec v1, vec const &v2) {
  for (size_t i = 0; i < v1.size(); ++i)
    v1[i] += v2[i];
  return v1;
}

vec operator*(int sc, vec v) {
  for (size_t i = 0; i < v.size(); ++i)
    v[i] *= sc;
  return v;
}

void part2() {
  auto particles = read();
  // Brute force potential collision times
  vector<pair<int, pair<size_t, size_t>>> potential_collisions;
  for (size_t i = 0; i < particles.size(); ++i)
    for (size_t j = i + 1; j < particles.size(); ++j) {
      auto tij = collision(particles[i], particles[j]);
      if (tij)
        potential_collisions.emplace_back(*tij, make_pair(i, j));
    }
  // Scan in time order to account for collisions destroying particles
  sort(potential_collisions.begin(), potential_collisions.end());
  set<size_t> collided;
  // Time when the last collision occurred
  int t_last = -1;
  for (auto [t, ij] : potential_collisions) {
    auto [i, j] = ij;
    if (t > t_last && (collided.count(i) || collided.count(j)))
      // At least one of the pair already collided at an earlier time
      continue;
    // Note that all collisions with the same t get included
    collided.insert(i);
    collided.insert(j);
    t_last = t;
  }
  cout << particles.size() - collided.size() << '\n';
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
