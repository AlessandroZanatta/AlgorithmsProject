//
// Created by Alessandro Zanatta on 7/7/20.
//

#ifndef ALGORITHMPROJECT_AVL_H
#define ALGORITHMPROJECT_AVL_H

#define BUFFER 10
#define ROTATE_LEFT 0
#define ROTATE_RIGHT 1

/**
 * Bst data structure to hold a BST
 */
struct Avl{
    struct Node * root;
};

/**
 * Node of a generic binary tree
 */
struct Node{
    int key;
    char value[BUFFER];
    int height;
    struct Node * left;
    struct Node * right;
    struct Node * parent;
};


struct Avl * create_avl();
void destroy_avl(struct Avl * avl);
struct Node * find(struct Avl * avl, int key);
void insert(struct Avl * avl, int key, char * value);
void show(struct Avl * avl);
void delete(struct Avl * avl, struct Node * node);

#endif //ALGORITHMPROJECT_AVL_H
