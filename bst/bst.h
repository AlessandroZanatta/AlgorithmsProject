//
// Created by Alessandro Zanatta on 7/7/20.
//

#ifndef ALGORITHMPROJECT_BST_H
#define ALGORITHMPROJECT_BST_H

#define BUFFER 30
/**
 * Bst data structure to hold a BST
 */
struct Bst{
    struct Node * root;
};

/**
 * Node of a generic binary tree
 */
struct Node{
    int key;
    char value[BUFFER];
    struct Node * left;
    struct Node * right;
    struct Node * parent;
};

struct Bst * create_bst();
void destroy_bst(struct Bst * bst);
struct Node * find(struct Bst * bst, int key);
void insert(struct Bst * bst, int key, char * value);
void delete(struct Bst * bst, struct Node * node);
void show(struct Bst * bst);
void in_order(struct Bst * bst);

#undef BUFFER
#endif //ALGORITHMPROJECT_BST_H
