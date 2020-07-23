//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "avl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    int buffer = 30;
    char input [buffer];
    char * token;
    char * value;
    int key;

    struct Avl * avl = create_avl();
    fgets(input, buffer, stdin);

    while(strcmp(input, "exit\n") != 0){

        // printf("Now executing: '%s'", input);
        token = strtok(input, " ");
        if(strcmp(token, "show\n") == 0){

            showAvl(avl);
        } else if(strcmp(token, "clear\n") == 0){

            destroy_avl(avl);
            avl = create_avl();
        } else if(strcmp(token, "insert") == 0) {

            key = atoi(strtok(NULL, " ")); // atoi is fine there
            value = strtok(NULL, " ");
            unsigned int length = strlen(value);
            value[length - 1] = '\0';

            insertAvl(avl, key, value);
        }else if(strcmp(token, "remove") == 0){

            key = atoi(strtok(NULL, " "));
            struct NodeAvl * node = findAvl(avl, key);
            deleteAvl(avl, node);
        } else if(strcmp(token, "find") == 0){ // findAvl

            key = atoi(strtok(NULL, " "));
            struct NodeAvl * node = findAvl(avl, key);
            printf("%s", node->value);
        } else {
            printf("ERROR, token not recognized!\n");
        }

        // get the new line
        fgets(input, buffer, stdin);
    }

    // printf("Exiting...\n");
    destroy_avl(avl);
    exit(0);
}