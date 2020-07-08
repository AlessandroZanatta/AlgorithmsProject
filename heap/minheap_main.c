//
// Created by Alessandro Zanatta on 7/8/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"
#define BUFFER 50

int main(int argc, char** argv){

    char command[BUFFER];
    char * token = (char *) malloc(BUFFER * sizeof(char));
    struct MinHeap * minheap = NULL;

    fgets(command, BUFFER, stdin);

    while(strcmp(command, "exit\n") != 0){
        token = strtok(command, " ");
        if(strcmp(token, "build") == 0){

            int * array = (int *) malloc(sizeof(int) * 100);
            if(array == NULL){
                perror("Error occurred in build");
                exit(-1);
            }
            int i = 1;
            while((token = strtok(NULL, " ")) != NULL){
                array[i] = atoi(token);
                i++;
            }

            minheap = buildMinHeap(array, i);
            printElements(minheap);
        } else if(strcmp(token, "length\n") == 0){

            if(minheap == NULL){
                minheap = createEmptyMinHeap();
            }

            printf("%d\n", heapsize(minheap));
            printElements(minheap);
        } else if(strcmp(token, "getmin\n") == 0){

            printf("%d\n", getMin(minheap));
            printElements(minheap);
        } else if(strcmp(token, "extract\n") == 0){

            extractMin(minheap);
            printElements(minheap);
        } else if(strcmp(token, "insert") == 0){

            int key = atoi(strtok(NULL, " "));
            insert(minheap, key);
            printElements(minheap);
        } else if(strcmp(token, "change") == 0){

            int position = atoi(strtok(NULL, " "));
            int key = atoi(strtok(NULL, " "));

            changeValue(minheap, position+1, key);
            printElements(minheap);
        } else {
            printf("ERROR: unrecognized operation!!!\n");
        }

        printf("\n");
        fgets(command, BUFFER, stdin);
    }

    // destroyMinHeap(minheap);
    // free(token);
}