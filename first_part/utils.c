//
// Created by Alessandro Zanatta on 7/11/20.
//

#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * Allocates an integer array of BUFFER size.
 * This function is useful to have a standard code and array length in a single place.
 */
int * allocate_array(){
    return (int *) malloc(sizeof(int) * BUFFER);
}

/**
 * Swaps a[x] and a[y]
 * @param a
 * @param x
 * @param y
 */
void swap_array(int * a, int x, int y){
    int temp = a[x];
    a[x] = a[y];
    a[y] = temp;
}

/**
 * Partitions the array a between start and end using as partition value the value in key_pos position
 * @param a
 * @param start
 * @param end
 * @param key_pos the position in the array of the key we want to use to partition the array a. Must be 0 <= start <= key_pos <= end <= array_length-1
 * @return the position of the key used to partition
 */
int partition(int * a, int start, int end, int key_pos){
    swap_array(a, end, key_pos); // take the key to the last place of the array

    int x = a[end];
    int i = start-1;
    for(int j = start; j <= end; j++){
        if(a[j] <= x){
            i++;
            swap_array(a, i, j);
        }
    }

    return i;
}

/**
 * Reads an array of values and transforms it in an actual integer array
 * @param a the array to save the result into
 * @param values a string with a set of values in the form "N N N N N N" where N is a number of arbitrary digits
 * @param start_from starts saving numbers into the array starting from this position of the array
 * @return returns the number of elements read
 */
int read_array(int * a, char * string, int start_from){
    int i = start_from, j=0, res, offset, value;
    do{
        res = sscanf(string+j,"%i %n",&value,&offset);
        if(res==1){
            a[i++] = value;
            j = j + offset;
        }
    } while(res==1);
    return i - start_from;
}