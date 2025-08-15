#include <string>
#include <vector>
#include "RBT.h"
using namespace std;

// simple record to hold one player
struct Player {
  string name;
  int score;
};

// What we show after each update
struct RankInfo {
  int score;            // player's score now
  int rank;             // 1 = best (highest score)
  int sameScoreCount;   // how many players with exactly this score (includes self)
  int totalPlayers;     // total players currently on the board
};

class Leaderboard {
public:
  Leaderboard();

  // add a new player or update an existing one.
  // if a player's score changes, remove old score from RBT and insert the new score.
  void addOrUpdate(const string& name, int score);

  // find a player's score; returns true if found.
  bool getScore(const string& name, int& outScore) const;

  // print all players descending by score.
  void printAll() const;

  // validate the red–black tree invariants (uses your RBT::validate()).
  bool validateTree() const;

  // compute rank info for one player; returns false if name not found.
  bool computeRank(const string& name, RankInfo& outInfo) const;

  // get nearby rows (descending). halfWindow = how many above and how many below.
  vector<Player> neighborsAround(const string& name, int halfWindow) const;

private:
  vector<Player> players;  // simple array of (name,score)
  RBT tree;                     // RBT holds scores so we can validate after updates

  // find index of a name in the vector (linear scan, simple and clear). Returns -1 if not found.
  int findIndexByName(const string& name) const;

  // build a copy of players sorted by descending score (for printing, rank windows, etc.).
  vector<Player> sortedDesc() const;
};