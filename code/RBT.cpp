/*Plese refer to the header file (RBT.h) for documentation of each method. */

#include "RBT.h"

/*==============================================
This file implements a standard Red–Black tree while
keeping the same structure and naming style as BST we've implemented before.

1) A red–black tree is a BST with extra coloring rules
    that keep the tree height ~log2(N) (so it stays fast).
2) New nodes start Red. The root must be Black.
3) There can never be two Red nodes in a row (parent+child).
4) Every path from a node to a NULL leaf has the same number
    of Black nodes (black height).
  5) After insert/remove, we "fix up" the tree using rotations
    and recoloring to restore these properties.*/




RBT::RBT(){
    // double pointer, same as BST
    root = new rb_node*;
    *root = NULL;
}

RBT::~RBT() {}

// --------------------------- helper functions --------------------------

// color_of return the color of a node
static inline RBColor color_of(const rb_node* n) {
    if (n != NULL) {
        return n->color;
    } 
    else {
        return RBColor::Black; // null leaf treated as black
    }
}

// rb_is_red returns true if node is red
static inline bool rb_is_red(const rb_node* n){
    if (color_of(n) == RBColor::Red) return true;
    return false;
}

// rb_is_black returns true if node is black
static inline bool rb_is_black(const rb_node* n){
    if (color_of(n) == RBColor::Black) return true;
    return false;
}

// set_color sets node color
static inline void set_color(rb_node* n, RBColor c){
    if (n != NULL) {
        n->color = c;
    }
}

// rb_minimum returns the minimum (left-most) node in a subtree.
static rb_node* rb_minimum(rb_node* n) {
    while (n != NULL && n->left != NULL) {
        n = n->left;
    }
    return n;;
}

// rb_transplant replaces subtree rooted at u with subtree v (u's parent now points to v).
// also updates v->parent. If u was the root, updates *root_pp.
static void rb_transplant(rb_node** root_pp, rb_node* u, rb_node* v) {
    if (root_pp == NULL || u == NULL) return;

    if (u->parent == NULL) {
        *root_pp = v;
    } 
    else if (u == u->parent->left) {
        u->parent->left = v;
    } 
    else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}
// --------------------------------------------------------------


// init_node initializes a RB node with Red color
rb_node* RBT::init_node(int data) {
  rb_node* n = new rb_node;
  n -> data  = data;
  n -> color = RBColor::Red;
  n -> parent = NULL;
  n -> left = NULL;
  n -> right = NULL;
  return n;
}

// get root and set root
rb_node* RBT::get_root() {
  if (root == NULL) return NULL;
  return *root;
}
void RBT::set_root(rb_node** new_root) {
  root = new_root;
}

// ----------------------------------------------------------------------------
// Rotations(Left and Right)
// ----------------------------------------------------------------------------
void RBT::RBTreeRotateLeft(rb_node* node){
    if (node == NULL){                // nothing to rotate
        return;
    }
    rb_node* r = node -> right;      // rightchild
    if (r  == NULL){
        return;                      // cannot rotate left without right child
    } 

    // move r->left to node->right
    node -> right = r -> left;
    if (r -> left != NULL) {
        r -> left -> parent = node; //fix parent pointer
    }
    // link r to node's old parent
    r -> parent = node -> parent;
    if (node->parent == NULL) {
    // node was root, rightchild now becomes root
        *root = r;
    }
    else if(node == node -> parent -> left){
        node -> parent -> left = r;
    }
    else{
        node -> parent -> right = r;
    }
    // put node under r->left
    r -> left = node;
    node -> parent = r;
}

// mirroring RBTreeRotateLeft
void RBT::RBTreeRotateRight(rb_node* node){
    if (node == NULL){
        return;
    }
    rb_node* l = node -> left;         // leftchild
    if (l == NULL){
        return;                       // cannot rotate right without left child
    } 
    // move l->right to node->left
    node -> left = l -> right;
    if (l -> right != NULL) {
        l -> right -> parent = node; // fix parent pointer of the moved subtree
    }
    // link l up to node's old parent
    l -> parent = node -> parent;
    if (node -> parent == NULL){
        *root = l;
    } 
    else if (node == node->parent->right) {
        node -> parent -> right = l;
    } 
    else {
        node -> parent -> left = l;
    }

    // put node under l->right
    l -> right = node;
    node -> parent = l;
}


// ----------------------------------------------------------------------------
// insert (BST insert + red–black rebalancing)
// 1) Do a normal BST insert same as BST to place the new node.
// 2) New node starts with Red. If this creates violations, we run
//    the fix-up(rebalancing) loop to repair colors and/or rotate.
// ----------------------------------------------------------------------------
void RBT::insert(rb_node* z){
    if (z == NULL){
        return;                     // ignore null input
    }

    // regular BST insert(same as BST)

    // empty tree, the first node becomes root
    if (*root == NULL){
        *root = z;
        z->color = RBColor::Black;  // root must be black
        return;
    }

    rb_node* y = NULL;              // parent of z
    rb_node* x = *root;             // walking cursor

    while (x != NULL){
        y = x;                      // last non-null
        if ((z -> data) <  (x -> data)){
            x = x -> left;          // go left
        }
        else{
            x = x -> right;        // go right
        }
    }
    z -> parent = y;              // attach parent
    if ((z -> data) < (y -> data)){
        y -> left = z;            // attach as left child
    }
    else{
        y->right = z;             // attach as right child
    }

    // insert fix-up(rebalance): 
    // for this critical process, I referenced algorithms from GeeksforGeeks 
    // and adjusted them to align with BST structure
    // reference: 
    // https://www.geeksforgeeks.org/cpp/red-black-tree-in-cpp/
    //..............................................................................

    while (z != *root && z->parent != NULL && rb_is_red(z->parent)) {
        rb_node* parent = z->parent;
        rb_node* grand = NULL;
        if (parent != NULL) {
            grand = parent->parent;
        }
        if (grand == NULL) {
            break;               // no grandparent, no fix needed
        }

        // case: parent is left child of grandparent → uncle is grandparent->right
        if (parent == grand->left) {
            rb_node* uncle = grand->right;
            // if uncle is red, recolor parent and uncle to black,
            // grand parent to red
            if (rb_is_red(uncle)) {
                set_color(parent, RBColor::Black);
                set_color(uncle,  RBColor::Black);
                set_color(grand,  RBColor::Red);
                z = grand;       // continue fixing up from grand
            } 
            else { // uncle is black
                // if left-right structure, rotate parent left
                if (z == parent->right) {
                    z = parent;         // move z up
                    RBTreeRotateLeft(z);// rotate z   
                    parent = z->parent; // reassign parent and grandparent
                    grand = NULL;
                        if (parent != NULL) {
                            grand = parent->parent;
                        }
                }
                // if left-left structure, recolor and rotate grandparent
                if (parent != NULL && grand != NULL) {
                    set_color(parent, RBColor::Black);
                    set_color(grand,  RBColor::Red);
                    RBTreeRotateRight(grand);
                }
            }
        } 
        else { // case: parent is right child of grandparent, uncle is the left child
            rb_node* uncle = grand->left;

            if (rb_is_red(uncle)) { // uncle red: recolor and move up
                set_color(parent, RBColor::Black);
                set_color(uncle,  RBColor::Black);
                set_color(grand,  RBColor::Red);
                z = grand;
            } 
            else { // right-left structure, rotate parent
                if (z == parent->left) {
                    z = parent;
                    RBTreeRotateRight(z);
                    parent = z->parent;
                    grand = NULL;
                        if (parent != NULL) {
                            grand = parent->parent;
                        }
                } // right-right structure, recolor and rotate grandparent
                if (parent != NULL && grand != NULL) {
                    set_color(parent, RBColor::Black);
                    set_color(grand,  RBColor::Red);
                    RBTreeRotateLeft(grand);
                }
            }
        }   
    }
    // check root is black
    if (*root != NULL) {
        set_color(*root, RBColor::Black);
    }
}

// same as BST insert_data
void RBT::insert_data(int data) {
  rb_node* n = init_node(data);
  insert(n);
}

// ---------------------------- Remove ------------------------------------
//
// I referenced the Zybooks's 6 cases as small helpers named from 
// RBTreeTryCase1 to RBTreeTryCase6, each returning true if it finishes the fix,
// or false to allow the main loop to continue.
//
//variables in helper functions:
// root_pp:root pointer pointer (address of the tree's root pointer).
//         We pass this so rotations inside helpers can update the root.    
// x:the node that currently carries the "double black".          
// xp:the parent of x 
// 
//
//   Case1: x is root: stop
//   Case2: sibling red: rotate parent to make sibling black
//   Case3: parent black, sibling black, sibling's children black: recolor sibling red; x=parent
//   Case4: parent red, sibling black, sibling's children black: recolor sibling red + parent black,stop
//   Case5: sibling black, near child red,  far child black: rotate sibling (to convert to case6)
//   Case6: sibling black, far child red: rotate parent, recolor, stop

// Case1: x is the root, color it black and stop
static bool RBTreeTryCase1(rb_node** root_pp, rb_node*& x, rb_node*& xp){
    rb_node* r = NULL;
    if (root_pp != NULL){
        r = *root_pp;
    }
    if (x == r) {
        if (x != NULL){
            x->color = RBColor::Black;
        } 
        return true; // fixed
    }
    return false;
}

// Case2: sibling is red: rotate parent and recolor sibling black
static bool RBTreeTryCase2(RBT* t, rb_node** root_pp, rb_node*& x, rb_node*& xp){
    (void)root_pp;  // avoid compiler warning

    rb_node* p;
    if (x != NULL){
        p = x->parent;
    } else{
        p = xp;
    } 
    if (p == NULL){
        return false;
    }

    bool x_is_left = false;
    if (x == p->left){
        x_is_left = true;
    } 

    rb_node* s; // sibling
    if (x_is_left){
        s = p->right;
    }
    else{
        s = p->left;
    } 

    if (rb_is_red(s)) {
        set_color(s, RBColor::Black);  // make sibling black
        set_color(p, RBColor::Red);    // parent becomes red
        if (x_is_left){
            t->RBTreeRotateLeft(p);    // rotate to move black sibling up
        }
        else{
            t->RBTreeRotateRight(p);
        }

        // after rotation, x's parent may change
        if (x != NULL){
            xp = x->parent;
        }
        else{
            xp = p;
        }
    }
    return false;                    // continue with later cases
}

// Case3: parent black, sibling black, both sibling's children black,
//        color sibling red and move x up to parent.
static bool RBTreeTryCase3(rb_node*& x, rb_node*& xp){
    rb_node* p;
    if (x != NULL){
        p = x->parent;
    } 
    else{
        p = xp;
    }
    if (p == NULL){
        return false;
    }

    bool x_is_left = false;
    if (x == p->left){
        x_is_left = true;
    }

    rb_node* s;
    if (x_is_left){
        s = p->right;
    }
    else{
        s = p->left;
    }

    rb_node* nearc = NULL;  // child of s towards x
    rb_node* farc  = NULL;  // child of s away from x
    if (x_is_left) {
        if (s != NULL){
            nearc = s->left;
        }
        if (s != NULL){
            farc  = s->right;
        }
    } 
    else {
        if (s != NULL){
            nearc = s->right;
        }
        if (s != NULL){
            farc  = s->left;
        }
    }

    bool cond_parent_black = rb_is_black(p);
    bool cond_s_black      = rb_is_black(s);
    bool cond_near_black   = rb_is_black(nearc);
    bool cond_far_black    = rb_is_black(farc);

    if (cond_parent_black && cond_s_black && cond_near_black && cond_far_black) {
        if (s != NULL){
            // give one black to sibling by painting it RED
            s->color = RBColor::Red; 
        }
        x  = p;                 // move double-black up
        if (x != NULL){
            xp = x->parent;
         }
        else{
            xp = NULL;
        }
        return true;            // continue loop with new x
    }
    return false;
}

// Case4: parent red, sibling black, sibling's children BLACK: 
//        recolor parent and sibling, and stop
static bool RBTreeTryCase4(rb_node*& x, rb_node*& xp){
    rb_node* p;
    if (x != NULL){
        p = x->parent;
    }
    else{
        p = xp;
    }
    if (p == NULL){
        return false;
    }

    bool x_is_left = false;
    if (x == p->left){
        x_is_left = true;
    }

    rb_node* s;
    if (x_is_left){
        s = p->right;
    }
    else{
        s = p->left;
    }           

    rb_node* nearc = NULL;
    rb_node* farc  = NULL;
    if (x_is_left) {
        if (s != NULL){
            nearc = s->left;
        }
        if (s != NULL){
            farc  = s->right;
        }
    }     
    else {
        if (s != NULL){
            nearc = s->right;
        }
        if (s != NULL){
            farc  = s->left;
        }
    }

    bool cond_parent_red  = rb_is_red(p);
    bool cond_s_black     = rb_is_black(s);
    bool cond_near_black  = rb_is_black(nearc);
    bool cond_far_black   = rb_is_black(farc);

    if (cond_parent_red && cond_s_black && cond_near_black && cond_far_black) {
        if (s != NULL){
            s->color = RBColor::Red;  // sibling becomes red
        }
        p->color = RBColor::Black;    // parent becomes black
        return true;                  // fixed
    }
    return false;
}

// Case5: sibling black, near child red, far child black: 
//        rotate sibling (prep for case 6)
static bool RBTreeTryCase5(RBT* t, rb_node*& x, rb_node*& xp) {
    rb_node* p;
    if (x != NULL){
        p = x->parent;
    }
    else{
        p = xp;
    }
    if (p == NULL){
        return false;
    }

    bool x_is_left = false;
    if (x == p->left){
        x_is_left = true;
    }

    rb_node* s;
    if (x_is_left){
        s = p->right;
    }
    else{
        s = p->left;
    }           

    rb_node* nearc = NULL;
    rb_node* farc  = NULL;
    if (x_is_left) {
        if (s != NULL){
            nearc = s->left;
        }
        if (s != NULL){
            farc  = s->right;
        }
    } 
    else {
        if (s != NULL){
            nearc = s->right;
        }
        if (s != NULL){
            farc  = s->left;
        }
    }

    if (rb_is_black(s) && rb_is_red(nearc) && rb_is_black(farc)) {
        set_color(nearc, RBColor::Black);   // move black down
        set_color(s, RBColor::Red);         // sibling becomes red
        if (x_is_left){
            t->RBTreeRotateRight(s);        // rotate sibling
        }
        else{
            t->RBTreeRotateLeft(s);
        }
    
    }
    return false; // not finished, proceeds to Case6
}

// Case6: sibling black, far child red: rotate parent toward x and recolor
static bool RBTreeTryCase6(RBT* t, rb_node*& x, rb_node*& xp) {
    rb_node* p;
    if (x != NULL){
        p = x->parent;
    }
    else{
        p = xp;
    }
    if (p == NULL){
        return false;
    }

    bool x_is_left = false;
    if (x == p->left){
        x_is_left = true;
    }

    rb_node* s;
    if (x_is_left){
        s = p->right;
    }
    else{
        s = p->left;
    }

    rb_node* farc = NULL;
    if (x_is_left) {
        if (s != NULL){
            farc = s->right;
        }
    } 
    else {
        if (s != NULL){
            farc = s->left;
        }
    }

    if (rb_is_black(s) && rb_is_red(farc)) {
        if (s != NULL){
            s->color = p->color;            // sibling takes parent color
        }
        p->color = RBColor::Black;          // parent becomes black
        if (farc != NULL){
            farc->color = RBColor::Black;   // far child becomes black
        }
        if (x_is_left){
            t->RBTreeRotateLeft(p);         // rotate parent
        }
        else{
            t->RBTreeRotateRight(p);
        }
        return true;                        // fixed
    }
    return false;
}

//---------------------------RBTreeRemove-----------------------------
//standard BST removal to remove z, but track the color of the node
//actually spliced out (y). If a black node was removed, we may need
//to fix a "double black" using the cases above.
void RBT::RBTreeRemove(rb_node* z) {
    if (z == NULL) return;      // nothing to remove

    // BST remove while tracking the removed color ----
    rb_node* y = z;
    RBColor y_orig = y->color;  // remember original color
    rb_node* x = NULL;          // child that replaces y 
    rb_node* xp = NULL;         // parent of x 

    if (z->left == NULL) {
        // only right child or none
        x  = z->right;
        xp = z->parent;
        rb_transplant(root, z, z->right);
    } 
    else if (z->right == NULL) {
        // only left child 
        x  = z->left;
        xp = z->parent;
        rb_transplant(root, z, z->left);
    } 
    else {
        // two children, use successor y = min (z -> right)
        y = rb_minimum(z->right);
        y_orig = y->color;       // y's color for fix-up
        x = y->right;            // x takes place
        if (y->parent == z) {
            // successor is direct child
            xp = y;              // x parent becomes y
            if (x != NULL){
                x->parent = y;
            }
        } 
        else {
            // move y's right subtree up
            xp = y->parent;
            rb_transplant(root, y, y->right);
            // put z -> right under y
            y->right = z->right;
                if (y->right != NULL){
                    y->right->parent = y;
                }
        }
        // put y in z place and take z's color
        rb_transplant(root, z, y);
        y->left = z->left;
        if (y->left != NULL){
            y->left->parent = y;
        }
        y->color = z->color;
    }

  

  // if we removed a BLACK node, we might have a double-black to repair
    if (y_orig == RBColor::Black) {
        while (true) {
        // r is the current root point
        rb_node* r = NULL;
        if (root != NULL){
            r = *root;
        }

        // break if x is the root
        if (x == r){
            break;
        }
    
        if (x != NULL && rb_is_red(x)){
            break;
        }
        // try each case 
        // Case 1
        if (RBTreeTryCase1(root, x, xp)){
            break;
        }

        // Case 2
        (void)RBTreeTryCase2(this, root, x, xp);

        // Case 3
        if (RBTreeTryCase3(x, xp)){
            continue;
        }

        // Case 4
        if (RBTreeTryCase4(x, xp)){
            break;
        }

        // Case 5
        (void)RBTreeTryCase5(this, x, xp);

        // Case 6
        if (RBTreeTryCase6(this, x, xp)){
            break;
        }

        // move upward if nothing matched to avoid loop
        rb_node* p;
        if (x != NULL){
            p = x->parent;
        }
        else{
            p = xp;
        }
        if (p == NULL){
            break;
        }
        x = p;
        if (x != NULL){
            xp = x->parent;
        }
        else{
            xp = NULL;
        }
    }
        // make sure x and root are black when finished
        if (x != NULL){
            set_color(x, RBColor::Black);
        }
        if (*root != NULL) {
            set_color(*root, RBColor::Black);
        }
    }
}

// remove by kay(data), same as BST structure
void RBT::remove(int data) {
  rb_node* z = get_node(get_root(), data);
  if (z == NULL){
    return;
  }
  RBTreeRemove(z);
}

// ---------------------------- utility functions --------------------------------

// contains returns true if subtree contains data
bool RBT::contains(rb_node* subt, int data) const {
    rb_node* c = subt;
    while (c != NULL) {
        if (data == c->data){
            return true;
        }
        if (data < c->data){
            c = c->left;
        }
        else{
            c = c->right;
        }
    }
    return false;
}

// get_node returns pointer to node with given key
rb_node* RBT::get_node(rb_node* subt, int data) const {
    rb_node* c = subt;
    while (c != NULL) {
        if (data == c->data){
            return c;
        }
        if (data < c->data){
            c = c->left;
        }
        else{
            c = c->right;
        }
    }
    return NULL;
}

// size counts nodes in subtree
int RBT::size(rb_node* subt) const {
    if (subt == NULL){
        return 0;
    }
    return 1 + size(subt->left) + size(subt->right);
}

// to_vector implements inorder traversal → push sorted keys into vec
void RBT::to_vector(rb_node* subt, vector<int>& vec) const {
    if (subt == NULL){
        return;
    }
    to_vector(subt->left, vec);
    vec.push_back(subt->data);
    to_vector(subt->right, vec);
}

// ------------------------------ validator ----------------------------------
// Validator checks the two main RBt invariants:
// 1) No two Red nodes in a row (parent/child).
// 2) All root-to-NULL paths have the same number of Black nodes.
static int rb_black_height(rb_node* n) {
    if (n == NULL){
        return 1;     // null leaves are black
    } 
    if (rb_is_red(n)) {
        // parent and child cannot both be red
        if (rb_is_red(n->left)) {
            return -1;
        }
        if (rb_is_red(n->right)){
            return -1;
        }
    }

    // recursively count black height
    int lh = rb_black_height(n->left);
    if (lh < 0){
        return -1; // left side broken
    }
    int rh = rb_black_height(n->right);
    if (rh < 0){
        return -1; // right side broken
    }
    if (lh != rh){ // height not equal
        return -1;
    }

    // if the node is Black, increment black
    if (n->color == RBColor::Black){
        return lh + 1;
    }
    return lh; 
}

// validate returns true if tree is empty or non-negative
bool RBT::validate() const {
  if (root == NULL || *root == NULL){ // empty
    return true;
  }
  if ((*root)->color != RBColor::Black){
    return false;  // root must be black
  }
  return rb_black_height(*root) > 0; // non-negative
}


