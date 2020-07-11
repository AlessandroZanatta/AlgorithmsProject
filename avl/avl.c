//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "avl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER 10

/**
 * Creates an empty AVL
 * @return the pointer to it (in the heap)
 */
struct Avl * create_avl(){
    struct Avl * new_avl = (struct Avl *) malloc(sizeof(struct Avl));
    new_avl->root = NULL;

    return new_avl;
}

/**
 * Creates a new node with the given key and all pointers to NULL
 * @param key the key of the node
 * @param value the value of the node
 * @return his pointer in the heap
 */
struct Node * create_node(int key, char * value){
    struct Node * new_node = (struct Node *) malloc(sizeof(struct Node));

    new_node->key = key;

    strncpy(new_node->value, value, BUFFER);
    new_node->value[BUFFER-1] = '\0';

    new_node->right = NULL;
    new_node->left = NULL;
    new_node->parent = NULL;
    new_node->height = 1; // nodes are always inserted in a NULL leaf, so the height of a new node is always 1

    return new_node;
}

/**
 * Utility function to retrieve the height of a node (this works correctly for NULLs too)
 * @param node
 * @return the height of the node or 0 if NULL
 */
int height(struct Node * node){
    if(node == NULL){
        return 0;
    } else {
        return node->height;
    }
}

/**
 * Computes the balance factor of a node as the difference between right subtree height and left subtree height
 * @param node
 * @return the balance factor of the given node. This is a number in [-2,2]
 */
int balanceFactor(struct Node * node){
    return height(node->left) - height(node->right);
}

/**
 * Utility to compute max of two numbers
 * @param x
 * @param y
 * @return max(x,y)
 */
int max(int x, int y){
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

/**
 * Recursively destroys a tree
 * @param node
 */
void destroy_tree(struct Node * node) {
    if(node != NULL){
        destroy_tree(node->left);
        destroy_tree(node->right);
        free(node);
    }
}

/**
 * De-allocates a rbt completely (every single Node)
 * @param rbt to delete
 */
void destroy_avl(struct Avl * avl){
    destroy_tree(avl->root);
    free(avl);
}

/**
 * Searches for a key in the given BST
 * @param avl the avl
 * @param key the key
 * @return NULL if the key does not exist, the node containing the given key if it exist
 */
struct Node * find(struct Avl * avl, int key){
    struct Node * x = avl->root;

    while(x != NULL && x->key != key){
        if(key > x->key){
            x = x->right;
        } else {
            x = x->left;
        }
    }

    return x;
}

/**
 * Finds the Node containing the biggest key in the subtree of node
 * @param node the subtree
 * @return the Node containing the biggest key among the ones in the subtree of
 */
struct Node * max_node(struct Node * node){
    while(node->right != NULL){
        node = node->right;
    }

    return node;
}

/**
 * Finds the Node containing the biggest key in the subtree of node
 * @param node the subtree
 * @return the Node containing the biggest key among the ones in the subtree of
 */
struct Node * min_node(struct Node * node){
    while(node->left != NULL){
        node = node->left;
    }

    return node;
}

/**
 * Rotates the tree using node x and given direction
 * @param avl the tree we're performing the rotation on
 * @param x the node we want to perform the rotation on
 * @param direction ROTATE_LEFT or ROTATE_RIGHT
 */
struct Node * rotate(struct Avl * avl, struct Node * x, short direction){

    if(direction == ROTATE_LEFT){
        if(x->parent == NULL){ // x is the root of the avl
            avl->root = x->right;
        } else {
            if(x->parent->left == x){
                x->parent->left = x->right;
            } else {
                x->parent->right = x->right;
            }
        }

        x->right->parent = x->parent;
        x->parent = x->right;
        x->right = x->right->left;

        if(x->right != NULL){
            x->right->parent = x;
        }

        x->parent->left = x;
    } else {
        if(x->parent == NULL){ // x is the root of the avl
            avl->root = x->left;
            x->left->parent = NULL;
        } else {
            if(x->parent->left == x){
                x->parent->left = x->left;
            } else {
                x->parent->right = x->left;
            }
        }

        x->left->parent = x->parent;
        x->parent = x->left;
        x->left = x->left->right;

        if(x->left != NULL){
            x->left->parent = x;
        }
        x->parent->right = x;

    }

    return x->parent; // this is the node that is now in the place that was once occupied by the node x
}

/**
 * Updates the heights of the node x
 * @param x
 */
void updateHeights(struct Node * x){

    if(x->right != NULL){
        x->right->height = 1 + max(height(x->right->right), height(x->right->left));
    }
    if(x->left != NULL){
        x->left->height = 1 + max(height(x->left->right),height(x->left->left));
    }
    x->height = 1 + max(height(x->right), height(x->left));
}

/**
 *
 * @param avl
 * @param node
 * @param key
 */
void fixAvl(struct Avl * avl, struct Node * node, int key) {

    if(node != NULL){
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = balanceFactor(node);

        struct Node * x = node;

        if(balance > 1 && node->left != NULL){
            if(key < node->left->key){ // LL-unbalance

                x = rotate(avl, node, ROTATE_RIGHT);
                updateHeights(x);
            } else if(key > node->left->key) { // LR-unbalance

                x = rotate(avl, node->left, ROTATE_LEFT);
                updateHeights(x);

                x = rotate(avl, node, ROTATE_RIGHT);
                updateHeights(x);
            }
        } else if(balance < -1 && node->right != NULL){
            if(key > node->right->key){ // RR-unbalance

                x = rotate(avl, node, ROTATE_LEFT);
                updateHeights(x);
            } else if(key < node->right->key) { // RL-unbalance

                x = rotate(avl, node->right, ROTATE_RIGHT);
                updateHeights(x);

                x = rotate(avl, node, ROTATE_LEFT);
                updateHeights(x);
            }
        }

        fixAvl(avl, x->parent, key);
    }
}

/**
 *
 * @param avl
 * @param node
 * @param key
 */
void fixAvlDelete(struct Avl * avl, struct Node * node) {

    if(node != NULL){
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = balanceFactor(node);

        struct Node * x = node;

        if(balance > 1 && node->left != NULL){
            if(balanceFactor(node->left) >= 0){ // LL-unbalance

                x = rotate(avl, node, ROTATE_RIGHT);
                updateHeights(x);
            } else if(balanceFactor(node->left) < 0) { // LR-unbalance

                x = rotate(avl, node->left, ROTATE_LEFT);
                updateHeights(x);

                x = rotate(avl, node, ROTATE_RIGHT);
                updateHeights(x);
            }
        } else if(balance < -1 && node->right != NULL){
            if(balanceFactor(node->right) <= 0){ // RR-unbalance

                x = rotate(avl, node, ROTATE_LEFT);
                updateHeights(x);
            } else if(balanceFactor(node->right) > 0) { // RL-unbalance

                x = rotate(avl, node->right, ROTATE_RIGHT);
                updateHeights(x);

                x = rotate(avl, node, ROTATE_LEFT);
                updateHeights(x);
            }
        }

        fixAvlDelete(avl, x->parent);
    }
}

/**
 * Inserts a node with the given key and value and fixes the AVL tree
 * @param avl
 * @param key
 * @param value
 */
void insert(struct Avl * avl, int key, char * value){
    struct Node * new_node = create_node(key, value);

    struct Node * y = NULL;
    struct Node * x = avl->root;

    while(x != NULL){
        y = x;
        if(key > x->key){
            x = x->right;
        } else {
            x = x->left;
        }
    }

    if(y == NULL){ // BST was empty then
        avl->root = new_node;
    } else {
        new_node->parent = y;

        if(key > y->key){
            y->right = new_node;
        } else {
            y->left = new_node;
        }
    }

    fixAvl(avl, new_node->parent, key);
}

/**
 * TODO this code is not the best but i do not remember the order of operations done to make this more compact (and readable)
 * @param avl the avl
 * @param node the node containing the key we want to delete
 */
void delete(struct Avl * avl, struct Node * node){

    if(node->left == NULL || node->right == NULL){ // at least one child is NULL, easier to solve!
        struct Node * x = NULL;

        if(node->left != NULL) {
            x = node->left;
        } else {
            x = node->right;
        }

        if(node == avl->root){
            avl->root = x;
            if(x != NULL){
                x->parent = NULL;
            }
            fixAvlDelete(avl, avl->root);
        } else {
            struct Node * y = node->parent;
            if(x != NULL){
                x->parent = y;
            }

            if(y->left == node){
                y->left = x;
            } else {
                y->right = x;
            }
            fixAvlDelete(avl, y);
        }

        free(node);
    } else { // it has two children, so i use the predecessor/successor (here i chose successor, but it does not matter)
        struct Node * real_delete = min_node(node->right);

        node->key = real_delete->key;
        strcpy(node->value, real_delete->value);

        // there should be a complete swap to make the delete procedure formally correct,
        // but this works as well and it takes less computational time

        delete(avl, real_delete);
    }
}

void show_rec(struct Node * node){
    if(node == NULL || strncmp(node->value, "", BUFFER) == 0){
        printf("NULL ");
    } else {
        printf("%d:%s:%d ", node->key, node->value, node->height);
        show_rec(node->left);
        show_rec(node->right);
    }
}

/**
 * Prints the given bst in reverse polish notation
 * @param bst
 */
void show(struct Avl * avl){
    show_rec(avl->root);
}



