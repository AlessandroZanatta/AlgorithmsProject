//
// Created by Alessandro Zanatta on 7/18/20.
//

#include "../bst/bst.h"
#include "../avl/avl.h"
#include "../rbt/rbt.h"
#include "utils.h"
#include <stdio.h>

#define ITERATIONS 60

/**
 * Computes the median time of a random generation of "elements" integers
 * @param resolution of the used clock
 * @param elements to generate
 * @param seed for prng MT
 * @return the median initialization time
 */
double getInitTime(double resolution, int elements, unsigned long seed){

    struct timespec start,end;
    double times [ITERATIONS];
    int number;
    int counter;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{
            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++){
                number = (int) genRandLong(&prng);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= resolution * ((1 / 0.005) + 1));
        times[i] = getDifference(start, end) / counter;

    }

    quicksortDouble(times, 0, ITERATIONS - 1);
    return times[(int) (ITERATIONS/2)];
}

/**
 * Performs operations to get median time and median absolute deviation
 * @param times the given times
 * @param result in here
 */
void elaborateTimes(double * times, double * result) {
    quicksortDouble(times, 0, ITERATIONS-1);

    result[0] = times[(int) (ITERATIONS / 2)];
    for(int i = 0; i < ITERATIONS; i++){
        times[i] = (result[0] - times[i]) >= 0 ? (result[0] - times[i]) : (result[0] - times[i])*(-1); // abs(time[0] - times[i])
    }

    quicksortDouble(times, 0, ITERATIONS-1);

    result[1] = times[(int) (ITERATIONS / 2)];
}

void getBstTime(double resolution, int elements, unsigned long seed, double initTime, double * result){

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Bst * bst = create_bst();

    for(int i = 0; i < ITERATIONS; i++){

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);

                if (find(bst, key) == NULL) {
                    insert(bst, key,""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;

            // destroy current bst (de-allocate everything) and allocate a new one
            destroy_bst(bst);
            bst = create_bst();
        } while(getDifference(start, end) <= (initTime + resolution * ((1 / 0.005) + 1)) );
        times[i] = getDifference(start, end) / counter - initTime;
    }

    elaborateTimes(times, result);
}

void getAvlTime(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Avl * avl = create_avl();

    for(int i = 0; i < ITERATIONS; i++){

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);

                if (findAvl(avl, key) == NULL) {
                    insertAvl(avl, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;

            // destroy current avl (de-allocate everything) and allocate a new one
            destroy_avl(avl);
            avl = create_avl();
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = getDifference(start, end) / counter - initTime;
    }

    elaborateTimes(times, result);
}

void getRbtTime(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Rbt * rbt = create_rbt();

    for(int i = 0; i < ITERATIONS; i++){

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);

                if (findRbt(rbt, key) == &NIL) {
                    insertRbt(rbt, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;

            // destroy current rbt (de-allocate everything) and allocate a new one
            destroy_rbt(rbt);
            rbt = create_rbt();
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = getDifference(start, end) / counter - initTime;
    }

    elaborateTimes(times, result);
}

int main(){
    double resolution = getMedianResolution();

    int elements = 500;
    double initTime;
    double bstTime [2];
    double avlTime [2];
    double rbtTime [2];
    FILE * output;

    unsigned long long seed = time(NULL); // get seed as the time since Unix Epoch

    printf("Resolution %.17g ns\n", resolution*1000000000);

    output = fopen("../second_part/times/basic_times.txt", "w");
    fprintf(output, "N,T1,D1,T2,D2,T3,D3\n");
    printf("N T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 40; i++){

        printf("%d ", elements);

        initTime = getInitTime(resolution, elements, seed); // time needed to chose "elements" pseudo-random integers (using MT)
        // printf("%.17g ", initTime);

        getBstTime(resolution, elements, seed, initTime, bstTime);
        printf("%.17g %.17g  ", bstTime[0], bstTime[1]);

        getAvlTime(resolution, elements, seed, initTime, avlTime);
        printf("%.17g %.17g ", avlTime[0], avlTime[1]);

        getRbtTime(resolution, elements, seed, initTime, rbtTime);
        printf("%.17g %.17g", rbtTime[0], rbtTime[1]);

        printf("\n");

        fprintf(output, "%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                elements,
                bstTime[0], bstTime[1],
                avlTime[0], avlTime[1],
                rbtTime[0], rbtTime[1]);

        elements = (int) (elements * 1.32); // doing so it reaches a length of about 5'000'000
    }

    fclose(output);

}