#ifndef BST_H__
#define BST_H__

#include <memory>
#include <string>
#include <vector>

using namespace std;

// bst_node is the binary search tree node structure.
struct bst_node {
  int data;
  bst_node* left;
  bst_node* right;
};

// Binary search tree:
//
// From any subtree node t, the left subtree's data values must be
// less than t's data value. The right subtree's data values must be
// greater than or equal to t's data value.
class BST {
public:
  // The constructor initializes class variables and pointers here if needed.
  // Set root to null.
  BST();

  // deconstructor - use this to clean up all memory that the BST has allocated
  // but not returned with the 'delete' keyword.
  ~BST();

  // init_node initializes a new bst_node from the heap using the given
  // data, and two NULL children, and returns a pointer to it.
  bst_node* init_node(int data);

  // insert places the new_node in a proper location in the tree while obeying
  // the invariant. On return, root points to the root of the tree.
  void insert(bst_node* new_node);

  // insert_data creates a new node with the given data value and inserts it
  // into the tree. STRONG HINT: insert_data should use the insert method.
  // ANOTHER STRONG HINT: consider using init_node(data) to create a bst_node*.
  void insert_data(int data);

  // This removes a node from the tree whose data value matches the input. If no
  // node in the tree contains the given data, this function has no effect.
  //
  // Implementation note: when removing a node, you often must do some pointer
  // adjustments involing either the predecessor or successor. The unit tests
  // expect you to use the successor in those cases.
  void remove(int data);

  // contains returns true if any node in the subtree pointed to by subt
  // contains the given data value, false otherwise.
  bool contains(bst_node* subt, int data);

  // get_node searches through the subtree pointed to by subt for a node that
  // contains the given data value. If such a node is found, a pointer
  // to it is returned. Otherwise this function returns NULL.
  bst_node* get_node(bst_node* subt, int data);

  // size returns the number of nodes in the subtree pointed to by subt. If the
  // tree is empty (t is NULL), it returns zero.
  int size(bst_node* subt);

  // to_vector fills an integer vector to reflect the contents of the subtree
  // pointed to by subt. Size of the filled array will be the same as the
  // subtree's size (found with the size() function), and the order of the array
  // elements are the same that is found during an INORDER traversal of the
  // subtree.
  //
  // Note: the vector "vec" will be passed to this function as an empty vector
  // and you can add elements to it by using push_back() member function (e.g.
  // vec.push_back(4) adds 4 to the end of the vector).
  void to_vector(bst_node* subt, vector<int>& vec);

  // This function is implemented for you. It returns the root pointer.
  bst_node* get_root();

  // This function is implemented for you. It sets a given pointer as the new
  // root pointer.
  void set_root(bst_node** new_root);

  // you can add add more public member variables and member functions here if
  // you need

private:
  // this double pointer always will point to the root pointer of the tree
  bst_node** root;
  // you can add add more private member variables and member functions here if
  // you need
};

#endif // BST_H__
