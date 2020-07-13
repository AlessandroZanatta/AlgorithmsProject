//
// Created by Alessandro Zanatta on 7/13/20.
//

#include "../mtwister/mtwister.h"
#include "utils.h"
#include "time.h"
#include <stdio.h>

#define ITERATIONS 25

/**
 * Computes the median time of a random initialization of a vector of given length
 * @param resolution of the used clock
 * @param length of the array
 * @param seed for prng MT
 * @return the median initialization time
 */
long getInitTime(long resolution, int length, unsigned long seed){

    struct timespec start,end;
    long times [ITERATIONS];
    int array [length];
    int counter = 0;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        do{
            prng = seedRand(seed);
            for(int j = 0; j < length; j++){
                array[j] = (int) genRandLong(&prng);
            }

            clock_gettime(CLOCK_MONOTONIC, &end);
            counter++;
        } while((double) (end.tv_nsec - start.tv_nsec) <= (double) resolution * ((1 / 0.005) + 1));
        times[i] = (end.tv_nsec - start.tv_nsec) / counter;

    }

    quicksortLong(times, 0, ITERATIONS - 1);
    return times[(int) (ITERATIONS/2)];
}


long * getQuickSelectTime(long resolution, int length, unsigned long seed){

}


int main(){
    long resolution = getMedianResolution();

    printf("Clock: %ld\n", resolution);

    int array_length = 100;
    for(int i = 0; i < 40; i++){
        printf("\n%d", array_length);

        array_length = (int) (array_length * 1.32); // doing so it reaches a length of about 5'000'000
    }
}
