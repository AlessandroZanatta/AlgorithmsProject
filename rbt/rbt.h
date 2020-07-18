//
// Created by Alessandro Zanatta on 7/7/20.
//

#ifndef ALGORITHMPROJECT_RBT_H
#define ALGORITHMPROJECT_RBT_H

#define BUFFER 10
#define RED 0
#define BLACK 1
#define ROTATE_LEFT 2
#define ROTATE_RIGHT 3
/**
 * Bst data structure to hold a RBT
 */
struct Rbt{
    struct NodeRbt * root;
};

/**
 * Node of a generic binary tree
 */
struct NodeRbt{
    int key;
    char value[BUFFER];
    short color;
    struct NodeRbt * left;
    struct NodeRbt * right;
    struct NodeRbt * parent;
};


struct Rbt * create_rbt();
void destroy_rbt(struct Rbt * rbt);
struct NodeRbt * find(struct Rbt * rbt, int key);
void insert(struct Rbt * rbt, int key, char * value);
void show(struct Rbt * rbt);

#endif //ALGORITHMPROJECT_RBT_H
