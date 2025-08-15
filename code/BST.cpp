/* Note: refer to the header file (BST.h) for documentation of each method. */

#include "BST.h"

BST::BST() {
  // Here is one way to implement the constructor. Keep or change it, up to you.
  root = new bst_node*;
  *root = NULL;
}

BST::~BST() {}

bst_node* BST::init_node(int data) { 
  bst_node* new_node = new bst_node;
  new_node->data = data;
  new_node->left = nullptr;
  new_node->right = nullptr;
  return new_node; 
  }

void BST::insert(bst_node* new_node) {
  if (new_node == nullptr){
    return;
  }
  // initialize new node to avoid seg fault
  new_node -> left = nullptr; 
  new_node -> right = nullptr;

  if (root == nullptr || *root == nullptr) {
    if (root != nullptr) {
      *root = new_node;
    }
    return;
  }

  // find the position to insert
  bst_node* cursor = *root;
  bst_node* parent = nullptr; 

  while (cursor != nullptr){
    parent = cursor;
    if (new_node -> data < cursor -> data){
      cursor = cursor -> left;
    }
    else{
      cursor = cursor -> right;
    }
  }

  if (new_node -> data < parent -> data){
    parent -> left = new_node;
  }
  else{
    parent -> right = new_node;
  }
}



void BST::insert_data(int data) {
  bst_node* new_node = init_node(data);

  insert(new_node);
}

void BST::remove(int data) {
  bst_node* parent = nullptr;
  bst_node* cursor = *root;
  while (cursor != nullptr){
    // Check if cursor has an equal key
    if (cursor -> data == data){
      if (cursor -> left == nullptr && cursor -> right == nullptr){
        // directly remove leaf:
        if (parent == nullptr) //cursor is root
        {
          root = nullptr; // remove root
        }
        else if (parent -> left == cursor){
          parent -> left = nullptr;  // remove left leaf
        }
        else{
          parent -> right = nullptr; // remove right leaf
        }
        return;
      }
      else if (cursor -> right == nullptr){ // node only has left child
        if (parent == nullptr){ // cursor is root
          *root = cursor -> left; // left child become root
        }
        else if (parent -> left == cursor){
          parent -> left =cursor -> left; 
        }
        else{
          parent -> right = cursor -> left;
        }
        return;
      }
      else if (cursor -> left == nullptr){ // node only has right child
        if (parent == nullptr){ // cursor is root
          *root = cursor -> right; // right child become root
        }
        else if (parent -> left ==cursor){
          parent -> left = cursor -> right;
        }
        else{
          parent -> right = cursor -> right;
        }
        return;
      }
      else{ //remove node with two child
        bst_node* successor = cursor -> right; // successor (leftmost child of right subtree)
        while (successor -> left != nullptr){
          successor = successor -> left;
        }
        cursor -> data = successor -> data; // copy successor's data to cursor
        parent =cursor;

        cursor = cursor -> right; // assign cursor and data to keep loop
        data = successor -> data;
      }
    }
    else if (cursor -> data < data){ //search right
      parent = cursor;
      cursor = cursor -> right;
    }
    else { //search left
      parent = cursor;
      cursor = cursor -> left;
    }
  }
  return; // not found
}

bool BST::contains(bst_node* subt, int data) {
  bst_node* cursor = subt;
  if (subt == nullptr){ // if subtree is empty, return false
    return false;
  }

  while (cursor != nullptr){
    if (cursor -> data == data){
      return true;
    }
    else if (cursor -> data > data){  // search left
        cursor = cursor -> left;
      }
    else {  // search right
        cursor = cursor -> right;
      }
  }
  return false;
}

bst_node* BST::get_node(bst_node* subt, int data) {
  bst_node* cursor = subt;
  if (subt == nullptr){  // if subtree is empty, return null
    return NULL;
  }

  while (cursor != nullptr){
    if (cursor -> data == data){
      return cursor;
    }
    else if (cursor -> data > data){ //search left
        cursor = cursor -> left;
      }
    else {  // search right
        cursor = cursor -> right;
      }
  }
  return NULL;
}


int BST::size(bst_node* subt) {
  bst_node* cursor = subt;
  if (subt == nullptr){  // if subtree is empty, return 0
    return 0;
  }
  int count = 1 + size(subt->left) + size(subt->right); // use size() function + root
  return count;
}

void BST::to_vector(bst_node* subt, vector<int>& vec) {
  if (subt == nullptr) {
    return;  // if empty subtree, nothing to add
    }
  // inorder sequence: left, root, right
  to_vector(subt->left, vec);      // left subtree 
  vec.push_back(subt->data);       // Add cursor node's data
  to_vector(subt->right, vec);     // right subtree
}

bst_node* BST::get_root() {
  // This function is implemented for you
  if (*root == NULL)
    return NULL;
  return *root;
}

void BST::set_root(bst_node** new_root) {
  // This function is implemented for you
  root = new_root;
}

