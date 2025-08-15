# Final Project — Red-Black Tree Leaderboard

This is a small command-line Leaderboard app backed by a **Red-Black Tree (RBT)**.
It allows users to enter a player **name** and **score**, and the app immediately shows the player’s **rank**, a small window of neighbors, and validates the tree’s invariants after each update. A simple test verifies core behavior.

## 1) About the Program 

### Project Goal

- Implement a balanced binary search tree **(Red-Black Tree)** for integers.

- Use it in a tiny **Leaderboard**: when the user adds/updates a player’s score, the app prints the player’s rank and other details.

- Provide tests and a validator to ensure the RBT obeys its invariants after inserts/removes.

### The RBT Data Structure (what it is)

A **Red-Black Tree** is a binary search tree with **extra coloring rules** *(Red and Black)* that keep it roughly balanced. That balance guarantees operations in **O(log n)** time:

- **Insert**: O(log n)

- **Remove**: O(log n)

- **Search**: O(log n)

### RBT Invariants (the rules it must always follow)

1. **Each node is either Red or Black.**

2. **The root is Black.**

3. **NULL leaves are considered Black.**

4. **No two consecutive Red nodes** on any parent→child edge.

5. **Equal black height**: every path from a node down to a NULL leaf has the same number of **Black nodes**.

If an operation breaks a rule, we **recolor** nodes and/or **rotate** small subtrees to restore all invariants.

### RBT Implementation (what’s in the code)

- **Node type** `rb_node` </p>
Stores `data` (int), `color` (`RBColor::Red` or `RBColor::Black`), and pointers `left`, `right`, `parent`.

- **Rotations** </p>
`RBTreeRotateLeft(...)` and `RBTreeRotateRight(...)` change local shape while keeping the inorder order. They are the “tools” to fix balance.

- **Insert** </p>

1. Insert like a normal BST (new node starts **Red**).
2. **Fix-up (rebalance) loop**: if we get a **red parent + red child**, we recolor/rotate until rules hold.
3. Make **root Black** at the end.

- **Remove** </p>

1. Remove the target like a BST (swap with successor if it has two children).
2. If a Black node was physically removed, we may create a **“double-black”** on the path; run **Cases 1–6** (standard RBT remove cases) to repair using recolors/rotations.

- **Transplant** and **Minimum** helpers </p>
`rb_transplant` replaces one subtree with another. `rb_minimum` finds the successor on delete.

- **Validation** </p>
`validate()` checks: root is black, **no red-red** parent/child, and **equal black height** using a recursive counter.

## 2)Running the Demo (Build & Test)

### Requirements

- CMake ≥ 3.16

- C++17 compiler (g++, clang++, or MSVC)

- VS Code with CMake Tools and C/C++ extensions

### Project Layout 

```bash
├─ code/
│  ├─ BST.h / BST.cpp
│  ├─ RBT.h / RBT.cpp        
│  ├─ Leaderboard.h / Leaderboard.cpp
├─ app/
│  └─ main.cpp               # interactive prompt
├─ tests/
│  └─ test_rank.cpp          # simple test
└─ CMakeLists.txt
```

### Build

```bash
cmake -S . -B build
cmake --build build -j
```

## Run the app

```bash
./build/app
```

## Example

```bash
RBT Leaderboard. Type 'help' for help.

name or command> bob 90

=== Result ===
Name : bob
Score: 90
Rank : 1 of 1
Same score count: 1

Around this rank:
 -> bob : 90

Tree check: VALID
```

you can also type two lines:

```bash
name or command> alice
score> 120
```

Other commands:

- `print` — show whole leaderboard (highest first)

- `validate` — check RBT invariants

- `help` — help text

- `exit` — quit

## Run the tests

```bash
./build/runtests/test_rank     # if the CMake puts tests here
```

## 3) How **Leaderboard.cpp** Works (main functions)

We keep the player list simple and user-friendly:

- **Storage**: `std::vector<Player>` where `Player { std::string name; int score; }`.

- **Ordering**: when needed (print / neighbors / rank window) we copy and `std::sort` by **descending score**.

- **RBT usage**: on each update we also adjust the RBT multiset of scores and call `validate()` to ensure invariants still hold. 

Functions:

- `addOrUpdate(name, score)`

  - If **new** name: push `{name, score}` and `tree.insert_data(score)`.

  - If **existing** name and score changed: update vector, `tree.remove(oldScore)`, then `tree.insert_data(score)`.

- `getScore(name, outScore)`
  
    Linear scan to find and return the current score.

- `printAll()`

    Makes a sorted copy (highest first) and prints `rank. name : score`.

- `validateTree()`

    Calls `tree.validate()` to check RBT invariants.

- `computeRank(name, RankInfo&)`
  
    Counts how many players have **strictly greater** score for the rank (`1 + greater`), counts **ties** (same score), and fills `totalPlayers`.

- `neighborsAround(name, halfWindow)`

    Builds a descending list and returns a small “window” of rows around the player (e.g., `halfWindow=2` returns 2 above + self + 2 below).  

## 4) Major RBT Functions (what they do)

### Construction & basic access

- `RBT::RBT()`

    Initializes an empty tree (*root = nullptr).

- `rb_node* get_root()` / `void set_root(rb_node** new_root)`

    Return or replace the top pointer. The project uses a double-pointer `rb_node** root` so helpers can reassign the actual tree root easily.(aligning with BST approach we have implemented before)

### Node creation

- `rb_node* init_node(int data)`

    Initiates a node, sets `data`, **color = Red**, and `left/right/parent = NULL`.
    (New nodes start Red by the standard RBT rule.)

### Rotations (shape change, BST order preserved)

- `RBTreeRotateLeft(rb_node* x)`
  
```bash
    x           r
    \   -->   /
    r       x
```

Moves r->left to x->right, fixes parent links, and makes r the parent of x.

- `RBTreeRotateRight(rb_node* x)`

    Mirror of left rotation.

### Insert

- `insert(rb_node* z)`

    1. **BST insert** of z under the proper parent.

    2. **Fix-up loop**: while parent is Red, there are 3 main cases (plus mirrors):

       - **Uncle Red** → recolor parent & uncle to Black, grandparent to Red, move up.

       - **Triangle** → rotate to make a line (e.g., Left-Right → rotate parent left).

       - **Line** → recolor parent Black, grandparent Red, rotate at grandparent.

    3. Make **root Black** at the end.

- `insert_data(int data)`
    Convenience wrapper: `init_node(data)` then `insert(...)` (same structure as `BST insert`).

### Remove

- `RBTreeRemove(rb_node* z)`

    1. Standard BST delete:

        - If `z` has ≤ 1 child, transplant directly.

        - If `z` has 2 children, swap with its **successor** (`rb_minimum(z->right)`), then remove.

    2. If the **removed** node was **Black**, we may create a **double-black** at `x`:

    Run **Cases 1–6** with sibling/parent/children checks:

  - **Case 1**: `x` is root → paint Black, done.

  - **Case 2**: sibling Red → rotate at parent to make sibling Black.

  - **Case 3**: parent Black, sibling Black, sibling’s kids Black → paint sibling Red, move up.

  - **Case 4**: parent Red, sibling Black, sibling’s kids Black → swap parent/sibling colors, done.

  - **Case 5**: sibling Black, **near** child Red, far child Black → rotate at sibling → to Case 6.

  - **Case 6**: sibling Black, **far** child Red → rotate at parent, recolor (fix).

  3. Ensure `x` and the **root** are **Black** at the end.

- `remove(int data)`
  
    Finds the node by key and calls `RBTreeRemove`.

### Helpers & utilities

- `rb_transplant(root_pp, u, v)`
    Replace node `u` with subtree `v`in the parent links (root/left/right), and set `v->parent = u->parent`.

- `rb_minimum(n)`
    Walk leftmost to find the minimum (successor helper).

- `contains(subtree, data)`, `get_node(subtree, data)`
    Simple search utilities.

- `size(subtree)`
    Counts nodes recursively.

- `to_vector(subtree, vec)`
    **Inorder traversal** into `vec` (sorted ascending).

- `validate()`
    Checks **root Black**, **no red-red**, and **equal black height** using a recursive helper that returns the black height or **-1** if an invariant is violated.

## 5)The Node — `rb_node`

```bash
enum class RBColor { Red, Black };

struct rb_node {
  int data;            // the key
  RBColor color;       // Red or Black
  rb_node* parent;     // parent pointer
  rb_node* left;       // left child (smaller keys)
  rb_node* right;      // right child (larger or equal keys)
};
```

Notes:

- **NULL pointers are considered Black** leaves.

- **New nodes start Red** in `init_node`.

- The tree keeps a **pointer to a pointer** for the root (`rb_node** root`).
    That makes it easy for rotations and transplants to replace the root itself like when rotating at the top.

### 6) Credits & References

I referenced algorithms from ZyBooks and GeeksforGeeks. In addtion, I referenced Cmake.org for writing CMakeLists file.

Previously implemented BST files are included for reference as well.

references:

<https://www.geeksforgeeks.org/cpp/red-black-tree-in-cpp/>

<https://cmake.org/cmake/help/book/mastering-cmake/chapter/Writing%20CMakeLists%20Files.html>

<https://www.markdownguide.org/basic-syntax/>
