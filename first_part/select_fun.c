//
// Created by Alessandro Zanatta on 7/12/20.
//

#include "select_fun.h"
#include "utils.h"
#include "../heap/minheap.h"
#include "../heap/maxheap.h"

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
        struct MinHeap * H1 = buildMinHeap(a, length);
        struct MinHeapPos * H2 = createEmptyMinHeapPos(length);

        struct Pair res;

        insertMinPos(H2, getMin(H1), 1);

        for(int i = 1; i <= k; i++){
            extractMinPos(H2, &res);
            int l = LEFT(res.pos);
            int r = RIGHT(res.pos);

            if(l <= heapsizeMin(H1)){
                insertMinPos(H2, getKeyOnPositionMin(H1, l), l);
            }

            if(r <= heapsizeMin(H1)){
                insertMinPos(H2, getKeyOnPositionMin(H1, r), r);
            }
        }

        int result = getMinPos(H2);

        // destroyMinHeap(H1);
        // destroyMinHeapPos(H2);

        return result;
    } else { // use max-heap (more efficient)
        struct MaxHeap * H1 = buildMaxHeap(a, length);
        struct MaxHeapPos * H2 = createEmptyMaxHeapPos(length);
        k = length - k; // we need to perform length - k iterations if we use maxheaps!

        struct Pair res;

        insertMaxPos(H2, getMax(H1), 1);

        for(int i = 1; i < k; i++){
            extractMaxPos(H2, &res);
            int l = LEFT(res.pos);
            int r = RIGHT(res.pos);

            if(l <= heapsizeMax(H1)){
                insertMaxPos(H2, getKeyOnPositionMax(H1, l), l);
            }

            if(r <= heapsizeMax(H1)){
                insertMaxPos(H2, getKeyOnPositionMax(H1, r), r);
            }
        }

        int result = getMaxPos(H2);

        destroyMaxHeap(H1);
        destroyMaxHeapPos(H2);

        return result;
    }
}

int medianselect_rec(int *a, int p, int q, int k) {

    int l = p;
    int r = q;
    int k1 = k;
    if(q-p < 5){
        quicksort(a, p, q);
        return a[k];
    } else {
        while(r-l >= 5) {

            int i = 0;
            // for blocks of 5 elements, sort them and take the median.
            // Here i swap in the first place (which is p, as i consider the array between p and q) to optimize space complexity (in place)
            while (i < (int) ((r - l + 1) / 5)) {
                quicksort(a, l + i * 5, l + i * 5 + 4);
                swap_array(a, l + i, l + i * 5 + 2);
                i++;
            }

            // I may have a block of < 5 elements, check if it exists and sort it
            if (((r - l + 1) % 5) != 0) {
                quicksort(a, l + i * 5, r);
                swap_array(a, l + i, l + i * 5 + (int) (r - (l + i * 5)) / 2);
                i++;
            }

            r = p+i-1;
            k1 = (int) ((r-l)/10) + l;
        }

        int M = a[k1];
        int pivot = partition_with_pivot(a, p, q, M);

        if(pivot == k){
            return M;
        } else {
            if(k < pivot) {
                return medianselect_rec(a, p, pivot-1, k);
            } else {
                return medianselect_rec(a, pivot+1, q, k);
            }
        }
    }
}

/**
 * Finds the k-th element in the array a (if a was ordered) using MedianSelect (median of medians) algorithm
 * @param a
 * @param length of the array a
 * @param k the position of the element we want in the sorted array
 * @return the value of this k-th element
 */
int medianselect(int * a, int length, int k){
    return medianselect_rec(a, 0, length-1, k);
}