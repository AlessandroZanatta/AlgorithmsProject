//
// Created by Alessandro Zanatta on 7/11/20.
//

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * Computes the time difference between two timespec structs
 * @param start
 * @param end
 * @return a timespec with seconds and nanoseconds diff computed
 */
struct timespec time_diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

/**
 * Converts nanoseconds to seconds
 * @param i
 * @return the seconds
 */
double seconds(double i) {
    return i / 1000000000;
}

/**
 * Gets the difference between two timespec and returns it as a double
 * @param start
 * @param end
 * @return
 */
double getDifference(struct timespec start, struct timespec end){

    double diff = 0;

    struct timespec res = time_diff(start, end);

    diff = (double) res.tv_sec + seconds((double) res.tv_nsec);

    return diff;
}



/**
 * Gets the resolution of the clock using a MONOTONIC clock (steady clock!)
 * @return the resolution
 */
double getResolution(){

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    do{
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    } while(getDifference(start, end) == 0);

    return getDifference(start, end);
}

/**
 * Computes the resolution of the clock multiple times to have a better precision
 * @return the median resolution
 */
double getMedianResolution(){
    double res [10000];
    for(int i = 0; i < 10000; i++){
        res[i] = getResolution();
    }

    quicksortDouble(res, 0, 10000 - 1);

    return (double) (res[(int) (10000 / 2)]); // return median
}

// -------------------------------------------------------------------------------------------------------
// Some functions from above adapted to work with the long data type, nothing too interesting or commented
// -------------------------------------------------------------------------------------------------------

void swap_arrayDouble(double *a, int i, int j) {
    double temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

int partitionDouble(double *a, int p, int q) {

    double x = a[q];
    int i = p-1;
    for(int j = p; j <= q; j++){
        if(a[j] <= x){
            i++;
            swap_arrayDouble(a, i, j);
        }
    }

    return i;
}

void quicksortDouble (double * a, int p, int q){
    if(p < q){
        int r = partitionDouble(a, p, q);
        quicksortDouble(a, p, r-1);
        quicksortDouble(a, r+1, q);
    }
}

// Mersenne Twister

/* An implementation of the MT19937 Algorithm for the Mersenne Twister
 * by Evan Sultanik.  Based upon the pseudocode in: M. Matsumoto and
 * T. Nishimura, "Mersenne Twister: A 623-dimensionally
 * equidistributed uniform pseudorandom number generator," ACM
 * Transactions on Modeling and Computer Simulation Vol. 8, No. 1,
 * January pp.3-30 1998.
 *
 * http://www.sultanik.com/Mersenne_twister
 */

#define UPPER_MASK		0x80000000
#define LOWER_MASK		0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000

void m_seedRand(MTRand* rand, unsigned long seed) {
    /* set initial seeds to mt[STATE_VECTOR_LENGTH] using the generator
     * from Line 25 of Table 1 in: Donald Knuth, "The Art of Computer
     * Programming," Vol. 2 (2nd Ed.) pp.102.
     */
    rand->mt[0] = seed & 0xffffffff;
    for(rand->index=1; rand->index<STATE_VECTOR_LENGTH; rand->index++) {
        rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffffffff;
    }
}

/**
* Creates a new random number generator from a given seed.
*/
MTRand seedRand(unsigned long seed) {
    MTRand rand;
    m_seedRand(&rand, seed);
    return rand;
}

/**
 * Generates a pseudo-randomly generated long.
 */
unsigned long genRandLong(MTRand* rand) {

    unsigned long y;
    static unsigned long mag[2] = {0x0, 0x9908b0df}; /* mag[x] = x * 0x9908b0df for x = 0,1 */
    if(rand->index >= STATE_VECTOR_LENGTH || rand->index < 0) {
        /* generate STATE_VECTOR_LENGTH words at a time */
        int kk;
        if(rand->index >= STATE_VECTOR_LENGTH+1 || rand->index < 0) {
            m_seedRand(rand, 4357);
        }
        for(kk=0; kk<STATE_VECTOR_LENGTH-STATE_VECTOR_M; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for(; kk<STATE_VECTOR_LENGTH-1; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+(STATE_VECTOR_M-STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rand->mt[STATE_VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[STATE_VECTOR_LENGTH-1] = rand->mt[STATE_VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
    y = rand->mt[rand->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_B;
    y ^= (y << 15) & TEMPERING_MASK_C;
    y ^= (y >> 18);
    return y;
}

/**
 * Generates a pseudo-randomly generated double in the range [0..1].
 */
double genRand(MTRand* rand) {
    return((double)genRandLong(rand) / (unsigned long)0xffffffff);
}