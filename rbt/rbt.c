//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "rbt.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// here i suppose -1 is not a valid key, might need to find a better way to do this tho...
struct Node NIL = {-1, {0,0,0,0,0,0,0,0,0,0}, BLACK, NULL, NULL, NULL};

/**
 * Creates an empty RBT
 * @return the pointer to it (in the heap)
 */
struct Rbt * create_rbt(){
    struct Rbt * new_rbt = (struct Rbt *) malloc(sizeof(struct Rbt));
    new_rbt->root = &NIL;

    return new_rbt;
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

    // initializes every node with 2 black nils (which are, in fact, a reference to the same NIL, more space efficient)
    new_node->right = &NIL;
    new_node->left = &NIL;

    new_node->color = RED; // always create a red node

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
 * De-allocates a rbt completely (every single Node)
 * @param rbt to delete
 */
void destroy_rbt(struct Rbt * rbt){
    destroy_tree(rbt->root);
    free(rbt);
}

/**
 * Searches for a key in the given BST
 * @param rbt the rbt
 * @param key the key
 * @return NULL if the key does not exist, the node containing the given key if it exist
 */
struct Node * find(struct Rbt * rbt, int key){
    struct Node * x = rbt->root;

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
 * Inverts the color of the given node
 * @param node
 */
void recolor(struct Node * node){
    if(node->color == RED){
        node->color = BLACK;
    } else {
        node->color = RED;
    }
}

/**
 * Rotates the tree using node x and given direction
 * @param x the node we want to effectuate the rotation on
 * @param direction ROTATE_LEFT or ROTATE_RIGHT
 */
void rotate(struct Rbt * rbt, struct Node * x, short direction){

    if(direction == ROTATE_LEFT){
        if(x->parent == NULL){ // x is the root of the rbt
            rbt->root = x->right;
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

        if(x->right != &NIL){
            x->right->parent = x;
        }

        x->parent->left = x;
    } else {
        if(x->parent == NULL){ // x is the root of the rbt
            rbt->root = x->left;
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

        if(x->left != &NIL){
            x->left->parent = x;
        }
        x->parent->right = x;
    }
}

/**
 * Fixes the RBT operating recolors/rotations (after an insertion)
 * @param node
 */
void fixRBT(struct Rbt * rbt, struct Node * node) {
    if(node->parent == NULL){ // node is root of rbt
        node->color = BLACK;
    } else if(node->parent->color != BLACK){ // if parent color is not black, rbt needs to be fixed

        if(node->parent->parent == NULL){
            node->parent->color = BLACK;
        } else {
            if(node->parent->parent->left == node->parent){
                if(node->parent->parent->right->color == RED){
                    recolor(node->parent);
                    recolor(node->parent->parent);
                    recolor(node->parent->parent->right);

                    fixRBT(rbt, node->parent->parent);
                } else {
                    if(node->parent->left != node){ // my red node and the black node are not opposite, rotate
                        rotate(rbt, node->parent, ROTATE_LEFT);
                        recolor(node);
                        recolor(node->parent);
                        rotate(rbt, node->parent, ROTATE_RIGHT);
                    } else {

                        recolor(node->parent);
                        recolor(node->parent->parent);
                        rotate(rbt, node->parent->parent, ROTATE_RIGHT);
                    }
                }
            } else {
                if(node->parent->parent->left->color == RED){
                    recolor(node->parent);
                    recolor(node->parent->parent);
                    recolor(node->parent->parent->left);

                    fixRBT(rbt, node->parent->parent);
                } else {
                    if(node->parent->right != node){
                        rotate(rbt, node->parent, ROTATE_RIGHT);
                        recolor(node);
                        recolor(node->parent);
                        rotate(rbt, node->parent, ROTATE_LEFT);
                    } else {

                        recolor(node->parent);
                        recolor(node->parent->parent);
                        rotate(rbt, node->parent->parent, ROTATE_LEFT);
                    }
                }
            }
        }
    }
}

/**
 * Inserts a new key in the given rbt. It assumes that the key is NOT already in the rbt!
 * @param rbt
 * @param key
 */
void insert(struct Rbt * rbt, int key, char * value){
    struct Node * new_node = create_node(key, value);

    struct Node * y = NULL;
    struct Node * x = rbt->root;

    while(x != &NIL){
        y = x;
        if(key > x->key){
            x = x->right;
        } else {
            x = x->left;
        }
    }

    if(y == NULL){ // RBT was empty then
        rbt->root = new_node;
        rbt->root->color = BLACK;
        // no need to fix anything
    } else {
        new_node->parent = y;

        if(key > y->key){
            y->right = new_node;
        } else {
            y->left = new_node;
        }

        fixRBT(rbt, new_node);
    }
}

void show_rec(struct Node * node){
    if(node == &NIL || strncmp(node->value, "", BUFFER) == 0){
        printf("NULL ");
    } else {
        printf("%d:%s:", node->key, node->value);
        if(node->color == RED){
            printf("red ");
        } else {
            printf("black ");
        }
        show_rec(node->left);
        show_rec(node->right);
    }
}

/**
 * Prints the given bst in reverse polish notation
 * @param bst
 */
void show(struct Rbt * rbt){
    show_rec(rbt->root);
}



