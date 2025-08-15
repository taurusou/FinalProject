#ifndef RB_TREE_HPP__
#define RB_TREE_HPP__

#include <vector>
#include <string>
#include <memory>

using namespace std;

// A red–black tree augments a BST with a color bit and rebalancing rules
// to ensure O(log n) height. This header keep consistent with BST.h format 
// so we can upgrade incrementally.

// color of a node in the red–black tree.
// reference: 
// https://www.geeksforgeeks.org/cpp/enum-classes-in-c-and-their-advantage-over-enum-datatype/
enum class RBColor { Red, Black };

// rb_node is the red–black tree node structure.
struct rb_node {
  int data;            // key (same as BST)
  RBColor color;       // Red or Black
  rb_node* parent;     // parent pointer (for rotations/fixups)
  rb_node* left;
  rb_node* right;
};

class RBT {
public:
  // Constructor and deconstructor same as BST
  RBT();
  ~RBT();

  // init_node initializes a new rb_node from the heap using the given data.
  // new nodes are created RED.
  rb_node* init_node(int data);

  // insert an existing node pointer into the tree, then
  // run red–black rebalancing.
  void insert(rb_node* new_node);

  // insert_data creates a new node with the given data value and inserts it
  // into the tree.
  void insert_data(int data);

  // remove the node Using the standard BST delete with successor replacement, then red–black fixups.
  void remove(int data);

   // same as BST 
  bool contains(rb_node* subt, int data) const;
  rb_node* get_node(rb_node* subt, int data) const;
  int size(rb_node* subt) const;
  void to_vector(rb_node* subt, vector<int>& vec) const; // inorder
  rb_node* get_root();
  void set_root(rb_node** new_root);

  // verify all red–black invariants(for testing).
  // returns true if:
  //  1) every node is RED or BLACK
  //  2) root is BLACK (if non-empty)
  //  3) all leaves (nullptr) are considered BLACK
  //  4) Red nodes have BLACK children (no two reds in a row)
  //  5) Every path from a node to descendant leaves has same black-height
  bool validate() const;

  void RBTreeRotateLeft(rb_node* n);
  
  void RBTreeRotateRight(rb_node* n);

private:
  // same as BST
  rb_node** root;
  

  void RBTreeInsert(rb_node* n);
  void RBTreeRemove(rb_node* n);
};

  #endif // RBT_H__