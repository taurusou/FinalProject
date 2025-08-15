#include <iostream>
#include <vector>
#include <string>
#include "Leaderboard.h"
using namespace std;

// if cond is false, print "[FAIL] <msg>" and exit with code 1 so CTest marks
// the test as failed.
static void expect(bool cond, const char* msg) {
  if (!cond) {
    cout << "[FAIL] " << msg << "\n";
    exit(1);
  }
}

int main() {
  Leaderboard lb;

  // insert some initial players and scores.
  // also update the underlying RBT (insert/remove) and keep it valid.
  lb.addOrUpdate("alice", 120);
  lb.addOrUpdate("bob",    80);
  lb.addOrUpdate("carl",  150);
  lb.addOrUpdate("dina",   80);
  lb.addOrUpdate("bob",   140); // update

  RankInfo r;
  // check players info
  expect(lb.computeRank("carl", r), "carl present");
  expect(r.score == 150, "carl score");
  expect(r.rank == 1 || r.rank == 2, "carl rank near top");

  expect(lb.computeRank("alice", r), "alice present");
  expect(r.score == 120, "alice score");

  expect(lb.computeRank("bob", r), "bob present");
  expect(r.score == 140, "bob score");

  // validateTree() calls RBT::validate() (root black, no red-red, equal black height).
  expect(lb.validateTree(), "RBT validate()");

  vector<Player> near = lb.neighborsAround("bob", 1);
  expect(!near.empty(), "neighbors non-empty");

  cout << "[PASS] rank tests\n";
  return 0;
}