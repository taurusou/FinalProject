#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Leaderboard.h"

using namespace std;

// simple helpers
static void print_help() {
  cout << "Commands:\n";
  cout << "  help      - show this help\n";
  cout << "  print     - show full leaderboard\n";
  cout << "  validate  - check red-black tree invariants\n";
  cout << "  exit      - quit\n\n";
  cout << "You can enter either:\n";
  cout << "  <name> <score>   (one line)\n";
  cout << "or:\n";
  cout << "  <name>           (then I'll prompt for score)\n";
}

static void print_neighbors(const vector<Player>& rows, const string& who) {
  for (size_t i = 0; i < rows.size(); i++) {
    bool isSelf = (rows[i].name == who);
    if (isSelf){
      cout << " -> ";
    }
    else{
      cout << "    ";
    }
    cout << rows[i].name << " : " << rows[i].score << "\n";
  }
}

// parse "<name> <score>" from a line.
// returns true and fills name/score if successful.
static bool parse_name_score_line(const string& line, string& name, int& score) {
  istringstream iss(line);
  string maybeName;
  int maybeScore;
  if (!(iss >> maybeName)){
    return false;}          // nothing on the line
  if (maybeName == "help" || maybeName == "print" ||
      maybeName == "validate" || maybeName == "exit" || maybeName == "quit") {
    return false; // it's a command, not a name+score
  }
  if (iss >> maybeScore) {
    name = maybeName;
    score = maybeScore;
    return true;                                    // parsed "name score"
  }
  return false;                                     // only a name (no score here)
}

int main() {
  Leaderboard lb;
  cout << "RBT Leaderboard. Type 'help' for help.\n";

  string line;
  while (true) {
    cout << "\nname or command> ";
    if (!getline(cin, line)){
      break;}       // EOF → exit
    if (line.size() == 0){
      continue;}                // empty line → reprompt

    // trim leading/trailing spaces (simple)
    while (!line.empty() && (line.back() == ' ' || line.back() == '\t')){
      line.pop_back();
    }
    size_t p = 0; 
    
    while (p < line.size() && (line[p] == ' ' || line[p] == '\t')){
      p++;
    }
    if (p > 0){
      line = line.substr(p);
    }

    // Commands
    if (line == "help"){ 
      print_help(); 
      continue; 
    }
    if (line == "print"){ 
      lb.printAll(); 
      continue; 
    }
    if (line == "validate"){ 
      cout << (lb.validateTree() ? "VALID\n" : "INVALID\n"); 
      continue; 
    }
    if (line == "exit" || line == "quit"){
      break;
    }

    // Try one-line "<name> <score>"
    string name;
    int score = 0;
    if (!parse_name_score_line(line, name, score)) {
      // treat the whole line as NAME and ask for score on the next line
      name = line;
      cout << "score> " << flush;
      string sline;
      if (!getline(cin, sline)) {
        break;
      }
      istringstream iss2(sline);
      if (!(iss2 >> score)) {
        cout << "Please enter an integer score.\n";
        continue;
      }
    }

    // update leaderboard
    lb.addOrUpdate(name, score);

    // compute and show rank info
    RankInfo info;
    bool ok = lb.computeRank(name, info);
    if (!ok) {
      cout << "Unexpected: player not found after update.\n";
      continue;
    }

    cout << "\n=== Result ===\n";
    cout << "Name : " << name << "\n";
    cout << "Score: " << info.score << "\n";
    cout << "Rank : " << info.rank << " of " << info.totalPlayers << "\n";
    cout << "Same score count: " << info.sameScoreCount << "\n";

    // Show neighbors (2 above, 2 below)
    vector<Player> around = lb.neighborsAround(name, 2);
    if (!around.empty()) {
      cout << "\nAround this rank:\n";
      print_neighbors(around, name);
    }

    // Optional: verify the RB tree after each change
    bool valid = lb.validateTree();
    cout << "\nTree check: " << (valid ? "VALID" : "INVALID") << "\n";
  }

  return 0;
}