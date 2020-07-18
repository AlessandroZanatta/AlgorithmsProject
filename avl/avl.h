//
// Created by Alessandro Zanatta on 7/7/20.
//

#ifndef ALGORITHMPROJECT_AVL_H
#define ALGORITHMPROJECT_AVL_H

#define BUFFER 10
#define ROTATE_LEFT 2
#define ROTATE_RIGHT 3

/**
 * Bst data structure to hold a BST
 */
struct Avl{
    struct NodeAvl * root;
};

/**
 * NodeAvl of a generic binary tree
 */
struct NodeAvl{
    int key;
    char value[BUFFER];
    int height;
    struct NodeAvl * left;
    struct NodeAvl * right;
    struct NodeAvl * parent;
};


struct Avl * create_avl();
void destroy_avl(struct Avl * avl);
struct NodeAvl * findAvl(struct Avl * avl, int key);
void insertAvl(struct Avl * avl, int key, char * value);
void showAvl(struct Avl * avl);
void deleteAvl(struct Avl * avl, struct NodeAvl * node);

#endif //ALGORITHMPROJECT_AVL_H
