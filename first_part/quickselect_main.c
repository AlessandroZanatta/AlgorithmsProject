//
// Created by Alessandro Zanatta on 7/11/20.
//

#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>

int quickselect_rec(int *a, int p, int q, int k) {
    if(p == q) {
        return a[p];
    } else {
        int r = partition(a, p, q, q);
        if (k == r) {
            return a[r];
        } else {
            if (k < r) { // the element we want is on the left of r
                return quickselect_rec(a, p, r - 1, k);
            } else { // the element is on the right
                return quickselect_rec(a, r + 1, q, k);
            }
        }
    }
}

/**
 * Finds the k-th element in the array a (if a was ordered)
 * @param a
 * @param length of the array a
 * @param k the position of the element we want in the sorted array
 * @return the value of this k-th element
 */
int quickselect(int * a, int length, int k){
    return quickselect_rec(a, 0, length-1, k);
}

int main(){
    char * input_string = (char *) malloc(sizeof(char) * CHAR_BUFFER);
    int * array = allocate_array();

    fgets(input_string, CHAR_BUFFER, stdin);
    int length = read_array(array, input_string);

    fgets(input_string, CHAR_BUFFER, stdin);
    int k = (int) strtol(strtok(input_string, " "), (char **) NULL, 10) - 1; // -1 as the test give us the position with a starting array position of 1

    printf("%d", quickselect(array, length, k));

    free(input_string);
    free(array);
}

