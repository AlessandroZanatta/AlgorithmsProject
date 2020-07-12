//
// Created by Alessandro Zanatta on 7/12/20.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "select_fun.h"


int main(){
    char * input_string = (char *) malloc(sizeof(char) * CHAR_BUFFER);
    int * array = allocate_array();

    fgets(input_string, CHAR_BUFFER, stdin);
    int length = read_array(array, input_string, 1);

    fgets(input_string, CHAR_BUFFER, stdin);
    int k = (int) strtol(strtok(input_string, " "), (char **) NULL, 10) - 1; // -1 as the test give us the position with a starting array position of 1

    printf("%d", heapselect(array, length, k));

    free(input_string);
    free(array);
}