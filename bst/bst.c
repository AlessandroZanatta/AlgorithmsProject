//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "bst.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER 10

/**
 * Creates an empty BST
 * @return the pointer to it (in the heap)
 */
struct Bst * create_bst(){
    struct Bst * new_bst = (struct Bst *) malloc(sizeof(struct Bst));
    new_bst->root = NULL;

    return new_bst;
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

    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
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
 * De-allocates a bst completely (every single Node)
 * @param bst to delete
 */
void destroy_bst(struct Bst * bst){
    destroy_tree(bst->root);
    free(bst);
}

/**
 * Searches for a key in the given BST
 * @param bst the bst
 * @param key the key
 * @return NULL if the key does not exist, the node containing the given key if it exist
 */
struct Node * find(struct Bst * bst, int key){
    struct Node * x = bst->root;

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
 * Inserts a new key in the given bst. It assumes that the key is NOT already in the bst!
 * @param bst
 * @param key
 */
void insert(struct Bst * bst, int key, char * value){
    struct Node * new_node = create_node(key, value);

    struct Node * y = NULL;
    struct Node * x = bst->root;

    while(x != NULL){
        y = x;
        if(key > x->key){
            x = x->right;
        } else {
            x = x->left;
        }
    }

    if(y == NULL){ // BST was empty then
        bst->root = new_node;
    } else {
        new_node->parent = y;

        if(key > y->key){
            y->right = new_node;
        } else {
            y->left = new_node;
        }
    }
}

/**
 * Finds the Node containing the biggest key in the subtree of node
 * @param node the subtree
 * @return the Node containing the biggest key among the ones in the subtree of
 */
struct Node * max(struct Node * node){
    while(node->left != NULL){
        node = node->left;
    }

    return node;
}

/**
 * Finds the successor of the given node
 * @param node
 * @return the pointer to the successor of the given Node
 */
struct Node * successor(struct Node * node){
    if(node->right != NULL){
        return max(node->right);
    } else {
        while(node->parent != NULL && node->parent->right != node){
            node = node->parent;
        }

        return node;
    }
}

/**
 * TODO this code is not the best but i do not remember the order of operations done to make this more compact (and readable)
 * @param bst the bst
 * @param node the node containing the key we want to delete
 */
void delete(struct Bst * bst, struct Node * node){

    if(node->left == NULL || node->right == NULL){ // at least one child is NULL, easier to solve!
        struct Node * x = NULL;

        if(node->left != NULL) {
            x = node->left;
        } else {
            x = node->right;
        }

        if(node == bst->root){
            bst->root = x;
            if(x != NULL){
                x->parent = NULL;
            }
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
        }

        free(node);
    } else { // it has two childs, so i use the predecessor/successor (here i chose successor, but it does not matter)
        struct Node * real_delete = successor(node);

        node->key = real_delete->key;
        strcpy(node->value, real_delete->value);

        // there should be a complete swap to make the delete procedure formally correct,
        // but this works as well and it takes less computational time

        delete(bst, real_delete);
    }
}

/**
 * Prints the sub-tree radicated in the given node in reverse polish notation
 * @param node
 */
void show_rec(struct Node * node){
    if(node == NULL || strncmp(node->value, "", BUFFER) == 0){
        printf("NULL ");
    } else {
        printf("%d:%s ", node->key, node->value);
        show_rec(node->left);
        show_rec(node->right);
    }
}

/**
 * Prints the given bst in reverse polish notation
 * @param bst
 */
void show(struct Bst * bst){
    show_rec(bst->root);
}



