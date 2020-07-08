//
// Created by Alessandro Zanatta on 7/7/20.
//

#ifndef ALGORITHMPROJECT_BST_H
#define ALGORITHMPROJECT_BST_H

#define BUFFER 30
#define RED 0
#define BLACK 1
#define ROTATE_LEFT 2
#define ROTATE_RIGHT 3
/**
 * Bst data structure to hold a BST
 */
struct Rbt{
    struct Node * root;
};

/**
 * Node of a generic binary tree
 */
struct Node{
    int key;
    char value[BUFFER];
    short color;
    struct Node * left;
    struct Node * right;
    struct Node * parent;
};


struct Rbt * create_rbt();
void destroy_rbt(struct Rbt * rbt);
struct Node * find(struct Rbt * rbt, int key);
void insert(struct Rbt * rbt, int key, char * value);
void show(struct Rbt * rbt);

#undef BUFFER
#endif //ALGORITHMPROJECT_BST_H
