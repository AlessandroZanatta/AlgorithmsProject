//
// Created by Alessandro Zanatta on 7/7/20.
//

#include "rbt.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    int buffer = 30;
    char input [buffer];
    char * token = (char *) malloc(buffer * sizeof(char));
    char * value = (char *) malloc(buffer * sizeof(char));
    int key;

    struct Rbt * rbt = create_rbt();
    fgets(input, buffer, stdin);

    while(strcmp(input, "exit\n") != 0){

        // printf("Now executing: '%s'", input);
        token = strtok(input, " ");
        if(strcmp(token, "show\n") == 0){

            show(rbt);
        } else if(strcmp(token, "clear\n") == 0){

            destroy_rbt(rbt);
            rbt = create_rbt();
        } else if(strcmp(token, "insert") == 0){

            key = atoi(strtok(NULL, " ")); // atoi is fine there
            value = strtok(NULL, " ");
            unsigned int length = strlen(value);
            value[length-1] = '\0';

            insert(rbt, key, value);
        } else if(strcmp(token, "find") == 0){ // find

            key = atoi(strtok(NULL, " "));
            struct Node * node = find(rbt, key);
            printf("%s", node->value);
        } else {
            printf("ERROR, token not recognized!\n");
        }

        // get the new line
        fgets(input, buffer, stdin);
    }

    // printf("Exiting...\n");
    //destroy_rbt(rbt);
    //free(token);
    //free(value);
    exit(0);
}