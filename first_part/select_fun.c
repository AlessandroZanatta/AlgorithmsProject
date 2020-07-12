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
        struct MinHeapPos * H2 = createEmptyMinHeapPos();

        int res[2];

        insertMinPos(H2, getMin(H1), 1);

        for(int i = 1; i <= k; i++){
            extractMinPos(H2, res);
            int l = LEFT(res[POS]);
            int r = RIGHT(res[POS]);

            if(l <= heapsizeMin(H1)){
                insertMinPos(H2, getKeyOnPositionMin(H1, l), l);
            }

            if(r <= heapsizeMin(H1)){
                insertMinPos(H2, getKeyOnPositionMin(H1, r), r);
            }
        }


        // destroyMinHeap(H1);
        // destroyMinHeapPos(H2);

        return getMinPos(H2);
    } else { // use max-heap (more efficient)
        struct MaxHeap * H1 = buildMaxHeap(a, length);
        struct MaxHeapPos * H2 = createEmptyMaxHeapPos();
        k = length - k; // we need to perform length - k iterations if we use maxheaps!
        int res[2];

        insertMaxPos(H2, getMax(H1), 1);

        for(int i = 1; i < k; i++){
            extractMaxPos(H2, res);
            int l = LEFT(res[POS]);
            int r = RIGHT(res[POS]);

            if(l <= heapsizeMax(H1)){
                insertMaxPos(H2, getKeyOnPositionMax(H1, l), l);
            }

            if(r <= heapsizeMax(H1)){
                insertMaxPos(H2, getKeyOnPositionMax(H1, r), r);
            }
        }

        return getMaxPos(H2);
    }
}