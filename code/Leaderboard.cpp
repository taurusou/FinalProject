#include "Leaderboard.h"
#include <iostream>
#include <algorithm>  // std::sort
using namespace std;

Leaderboard::Leaderboard() : players(), tree() {}

// Find index by name using a simple loop
int Leaderboard::findIndexByName(const string& name) const {
  for (size_t i = 0; i < players.size(); i++) {
    if (players[i].name == name) {
      return (int)i;
    }
  }
  return -1;
}

// Simple comparator: higher score first. If equal, keep relative order (no need for stable_sort here).
static bool score_desc(const Player& a, const Player& b) {
  if (a.score > b.score) return true;
  if (a.score < b.score) return false;
  return false;
}

vector<Player> Leaderboard::sortedDesc() const {
  vector<Player> copy = players;               // work on a copy
  sort(copy.begin(), copy.end(), score_desc);  // highest score first
  return copy;
}

void Leaderboard::addOrUpdate(const string& name, int score) {
  int idx = findIndexByName(name);
  if (idx >= 0) {
    int old = players[(size_t)idx].score;
    if (old != score) {
      // update player record
      players[(size_t)idx].score = score;
      // update RBT multiset: remove one occurrence of old, insert one of new
      tree.remove(old);
      tree.insert_data(score);
    }
  } else {
    // new player → push to vector and insert score into RBT
    Player p;
    p.name = name;
    p.score = score;
    players.push_back(p);
    tree.insert_data(score);
  }
}

bool Leaderboard::getScore(const string& name, int& outScore) const {
  int idx = findIndexByName(name);
  if (idx < 0) return false;
  outScore = players[(size_t)idx].score;
  return true;
}

void Leaderboard::printAll() const {
  vector<Player> s = sortedDesc();
  cout << "=== Leaderboard (highest first) ===\n";
  for (size_t i = 0; i < s.size(); i++) {
    cout << (i + 1) << ". " << s[i].name << " : " << s[i].score << "\n";
  }
}

bool Leaderboard::validateTree() const {
  return tree.validate();
}

bool Leaderboard::computeRank(const string& name, RankInfo& outInfo) const {
  int idx = findIndexByName(name);
  if (idx < 0) return false;  // player not found

  int sc = players[(size_t)idx].score;

  // Compute rank with simple counting: how many strictly greater + count ties
  int greater = 0;
  int ties = 0;
  for (size_t i = 0; i < players.size(); i++) {
    if (players[i].score > sc) {
      greater = greater + 1;
    } else if (players[i].score == sc) {
      ties = ties + 1;
    }
  }

  outInfo.score = sc;
  outInfo.rank = greater + 1;          // 1-based rank
  outInfo.sameScoreCount = ties;       // includes self
  outInfo.totalPlayers = (int)players.size();
  return true;
}

vector<Player> Leaderboard::neighborsAround(const string& name, int halfWindow) const {
  vector<Player> out;
  if (players.empty()) return out;

  // Get a descending view and find the position of "name"
  vector<Player> s = sortedDesc();

  int pos = -1;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i].name == name) {
      pos = (int)i;
      break;
    }
  }
  if (pos < 0) return out; // not found in the sorted view (shouldn't happen)

  // Compute window bounds safely
  int start = pos - halfWindow;
  if (start < 0) start = 0;
  int end = pos + halfWindow;
  if (end >= (int)s.size()) end = (int)s.size() - 1;

  // Collect that window
  for (int i = start; i <= end; i++) {
    out.push_back(s[(size_t)i]);
  }
  return out;
}