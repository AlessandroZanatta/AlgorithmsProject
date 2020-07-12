//
// Created by Alessandro Zanatta on 7/12/20.
//

#include "select_fun.h"
#include "utils.h"

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
 * Finds the k-th element in the array a (if a was ordered) using QuickSelect algorithm
 * @param a
 * @param length of the array a
 * @param k the position of the element we want in the sorted array
 * @return the value of this k-th element
 */
int quickselect(int * a, int length, int k){
    return quickselect_rec(a, 0, length-1, k);
}


/**
 * Finds the k-th element in the array a (if a was ordered) using HeapSelect algorithm
 * @param a
 * @param length of the array a
 * @param k the position of the element we want in the sorted array
 * @return the value of this k-th element
 */
int heapselect(int * a, int length, int k){
    if(k <= (int) (length / 2)){ // use min-heap (more-efficient)

    } else { // use max-heap (more efficient)

    }
}