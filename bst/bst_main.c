//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "bst.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    int buffer = 30;
    char input [buffer];
    char * token;
    char * value;
    int key;

    struct Bst * bst = create_bst();
    fgets(input, buffer, stdin);

    while(strcmp(input, "exit\n") != 0){

        // printf("Now executing: '%s'", input);
        token = strtok(input, " ");
        if(strcmp(token, "show\n") == 0){
            show(bst);
        } else if(strcmp(token, "clear\n") == 0){

            destroy_bst(bst);
            bst = create_bst();
        } else if(strcmp(token, "insert") == 0){

            key = atoi(strtok(NULL, " ")); // atoi is fine there
            value = strtok(NULL, " ");

            insert(bst, key, value);
        } else if(strcmp(token, "remove") == 0){

            key = atoi(strtok(NULL, " "));
            struct Node * node_with_key = find(bst, key);

            delete(bst, node_with_key);
        } else if(strcmp(token, "find") == 0){ // find

            key = atoi(strtok(NULL, " "));
            struct Node * node = find(bst, key);
            printf("%s", node->value);
        } else {
            printf("ERROR, token not recognized!\n");
        }

        // get the new line
        fgets(input, buffer, stdin);
    }

    // printf("Exiting...\n");
    destroy_bst(bst);
    exit(0);
}