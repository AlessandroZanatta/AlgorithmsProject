//
// Created by Alessandro Zanatta on 7/18/20.
//

#include "../bst/bst.h"
#include "../avl/avl.h"
#include "../rbt/rbt.h"
#include "utils.h"
#include <stdio.h>

#define ITERATIONS 10
#define RANDOM_PROBABILITY 0.95
#define POINTERS 1000


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


double getInitTimeOrdered(double resolution, int elements, unsigned long long int seed) {
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
                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    number = (int) genRandLong(&prng);
                } else {
                    number = j;
                }
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

void deallocateBST(struct Bst *const *pointers, int counter) {
    if(counter > POINTERS){
        printf("@@@@@@@@@@@@@@@@ NEED MORE SPACE FOR POINTERS: %d @@@@@@@@@@@@@@@@@", counter);
    }

    // destroy ALL bst(s) allocated in the last iteration
    for(int j = 0; j < counter; j++){
        destroy_bst(pointers[j]);
    }
}

void getBstTime(double resolution, int elements, unsigned long seed, double initTime, double * result){


    struct timespec start,end;
    double times [ITERATIONS];
    struct Bst * pointers[POINTERS]; // this is used to remember all memory the program has to free

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

            pointers[counter] = bst; // free-ing the memory while taking times will produce false results, so i save pointers and free them at the end of the cycle
            bst = create_bst(); // allocate new bst
            counter++;
        } while(getDifference(start, end) <= (initTime + resolution * ((1 / 0.005) + 1)) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;

        deallocateBST(pointers, counter);

    }

    elaborateTimes(times, result);
}

void deallocateAVL(struct Avl *const *pointers, int counter) {
    if(counter > POINTERS){
        printf("@@@@@@@@@@@@@@@@ NEED MORE SPACE FOR POINTERS: %d @@@@@@@@@@@@@@@@@", counter);
    }

    for(int j = 0; j < counter; j++){
        destroy_avl(pointers[j]);
    }
}

void getAvlTime(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];
    struct Avl * pointers[POINTERS];

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

            pointers[counter] = avl;
            avl = create_avl();
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;

        deallocateAVL(pointers, counter);
    }

    elaborateTimes(times, result);
}

void deallocateRBT(struct Rbt *const *pointers, int counter) {
    if(counter > POINTERS){
        printf("@@@@@@@@@@@@@@@@ NEED MORE SPACE FOR POINTERS: %d @@@@@@@@@@@@@@@@@", counter);
    }

    for(int j = 0; j < counter; j++){
        destroy_rbt(pointers[j]);
    }
}

void getRbtTime(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];
    struct Rbt * pointers [POINTERS];

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

            pointers[counter] = rbt;
            rbt = create_rbt();
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;

        deallocateRBT(pointers, counter);
    }

    elaborateTimes(times, result);
}

void getBstTimeOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];
    struct Bst * pointers[POINTERS];

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

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }

                if (find(bst, key) == NULL) {
                    insert(bst, key,""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            pointers[counter] = bst;
            bst = create_bst();
            counter++;
        } while(getDifference(start, end) <= (initTime + resolution * ((1 / 0.005) + 1)) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        deallocateBST(pointers, counter);
    }

    elaborateTimes(times, result);
}

void getAvlTimeOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];
    struct Avl * pointers [POINTERS];

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

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }

                if (findAvl(avl, key) == NULL) {
                    insertAvl(avl, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            pointers[counter] = avl;
            avl = create_avl();
            counter++;
        } while(getDifference(start, end) <= (initTime + resolution * ((1 / 0.005) + 1)) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        deallocateAVL(pointers, counter);
    }

    elaborateTimes(times, result);
}

void getRbtTimeOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];
    struct Rbt * pointers[POINTERS];

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

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }

                if (findRbt(rbt, key) == &NIL) {
                    insertRbt(rbt, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
                }
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            pointers[counter] = rbt;
            rbt = create_rbt();
            counter++;
        } while(getDifference(start, end) <= (initTime + resolution * ((1 / 0.005) + 1)) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        deallocateRBT(pointers, counter);
    }

    elaborateTimes(times, result);
}


void getBstTimeFind(double resolution, int elements, unsigned long seed, double initTime, double * result){

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;
    struct Bst * bst;

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        bst = create_bst();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            key = (int) genRandLong(&prng);
            if(find(bst, key) == NULL){
                insert(bst, key, "");
            }
        }

        // find the keys we just inserted and compute time
        // since initialization is done only once (as finds do not change the tree), it is not included in the computed time
        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);
                find(bst, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_bst(bst);
    }

    elaborateTimes(times, result);
}

void getAvlTimeFind(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;
    struct Avl * avl;

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        avl = create_avl();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            key = (int) genRandLong(&prng);
            if(findAvl(avl, key) == NULL){
                insertAvl(avl, key, "");
            }
        }

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);
                findAvl(avl, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_avl(avl);
    }

    elaborateTimes(times, result);
}

void getRbtTimeFind(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {

    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Rbt * rbt;

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        rbt = create_rbt();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            key = (int) genRandLong(&prng);
            if(findRbt(rbt, key) == &NIL){
                insertRbt(rbt, key, "");
            }
        }

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                key = (int) genRandLong(&prng);
                findRbt(rbt, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_rbt(rbt);
    }

    elaborateTimes(times, result);
}

void getBstTimeFindOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Bst * bst;

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        bst = create_bst();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                key = (int) genRandLong(&prng);
            } else {
                key = j;
            }

            if (find(bst, key) == NULL) {
                insert(bst, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
            }
        }

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }
                find(bst, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_bst(bst);
    }

    elaborateTimes(times, result);
}

void getAvlTimeFindOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Avl * avl;

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        avl = create_avl();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                key = (int) genRandLong(&prng);
            } else {
                key = j;
            }

            if (findAvl(avl, key) == NULL) {
                insertAvl(avl, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
            }
        }

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }
                findAvl(avl, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_avl(avl);
    }

    elaborateTimes(times, result);
}

void getRbtTimeFindOrdered(double resolution, int elements, unsigned long long int seed, double initTime, double result[2]) {
    struct timespec start,end;
    double times [ITERATIONS];

    int counter;
    int key;
    MTRand prng;

    struct Rbt * rbt = create_rbt();

    for(int i = 0; i < ITERATIONS; i++){

        // create tree
        rbt = create_rbt();
        prng = seedRand(seed+i);
        for(int j = 0; j < elements; j++){
            if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                key = (int) genRandLong(&prng);
            } else {
                key = j;
            }

            if (findRbt(rbt, key) == &NIL) {
                insertRbt(rbt, key, ""); // insert just an empty string, requires less computation (plus: what should I even write here anyway?)
            }
        }

        counter = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{

            prng = seedRand(seed+i);
            for(int j = 0; j < elements; j++) {

                if(genRand(&prng) <= RANDOM_PROBABILITY){ // with RANDOM_PROBABILITY probability, generate a random number, otherwise use an ordered sequence
                    key = (int) genRandLong(&prng);
                } else {
                    key = j;
                }
                findRbt(rbt, key);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= initTime + resolution * ((1 / 0.005) + 1) );
        times[i] = (getDifference(start, end) / counter - initTime) / elements;
        destroy_rbt(rbt);
    }

    elaborateTimes(times, result);
}

int main(){
    double resolution = getMedianResolution();

    int elements = 100;
    double initTime;
    double bstTime [2];
    double avlTime [2];
    double rbtTime [2];
    FILE * output;

    unsigned long long seed = time(NULL); // get seed as the time since Unix Epoch

    printf("Resolution %.17g ns\n", resolution*1000000000);
/*

    // basic comparation of the 3 trees
    output = fopen("second_part/times/all.txt", "w");
    fprintf(output, "N,T1,D1,T2,D2,T3,D3\n");
    printf("N T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 35; i++){

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

        elements = (int) (elements * 1.31);
    }
    fclose(output);


    elements = 100;
    // pseudo-ordered input sequence
    // 95% random elements
    output = fopen("second_part/times/all_ordered.txt", "w");
    fprintf(output, "N,T1,D1,T2,D2,T3,D3\n");
    printf("N T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 35; i++){

        printf("%d ", elements);

        initTime = getInitTimeOrdered(resolution, elements, seed);

        getBstTimeOrdered(resolution, elements, seed, initTime, bstTime);
        printf("%.17g %.17g  ", bstTime[0], bstTime[1]);

        getAvlTimeOrdered(resolution, elements, seed, initTime, avlTime);
        printf("%.17g %.17g ", avlTime[0], avlTime[1]);

        getRbtTimeOrdered(resolution, elements, seed, initTime, rbtTime);
        printf("%.17g %.17g", rbtTime[0], rbtTime[1]);

        printf("\n");

        fprintf(output, "%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                elements,
                bstTime[0], bstTime[1],
                avlTime[0], avlTime[1],
                rbtTime[0], rbtTime[1]);

        elements = (int) (elements * 1.31);
    }
    fclose(output);


    // try to effectuate "elements" find (ONLY FIND)
    elements = 100;
    output = fopen("second_part/times/finds.txt", "w");
    fprintf(output, "N,T1,D1,T2,D2,T3,D3\n");
    printf("N T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 40; i++){

        printf("%d ", elements);

        initTime = getInitTime(resolution, elements, seed);

        getBstTimeFind(resolution, elements, seed, initTime, bstTime);
        printf("%.17g %.17g  ", bstTime[0], bstTime[1]);

        getAvlTimeFind(resolution, elements, seed, initTime, avlTime);
        printf("%.17g %.17g ", avlTime[0], avlTime[1]);

        getRbtTimeFind(resolution, elements, seed, initTime, rbtTime);
        printf("%.17g %.17g", rbtTime[0], rbtTime[1]);

        printf("\n");

        fprintf(output, "%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                elements,
                bstTime[0], bstTime[1],
                avlTime[0], avlTime[1],
                rbtTime[0], rbtTime[1]);

        elements = (int) (elements * 1.31);
    }
    fclose(output);


    // try to effectuate "elements" find (ONLY FIND) with pseudo-ORDERED inserted elements!
    // 95% random elements
    elements = 100;
    output = fopen("second_part/times/finds_ordered.txt", "w");
    fprintf(output, "N,T1,D1,T2,D2,T3,D3\n");
    printf("N T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 35; i++){

        printf("%d ", elements);

        initTime = getInitTimeOrdered(resolution, elements, seed);

        getBstTimeFindOrdered(resolution, elements, seed, initTime, bstTime);
        printf("%.17g %.17g  ", bstTime[0], bstTime[1]);

        getAvlTimeFindOrdered(resolution, elements, seed, initTime, avlTime);
        printf("%.17g %.17g ", avlTime[0], avlTime[1]);

        getRbtTimeFindOrdered(resolution, elements, seed, initTime, rbtTime);
        printf("%.17g %.17g", rbtTime[0], rbtTime[1]);

        printf("\n");

        fprintf(output, "%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                elements,
                bstTime[0], bstTime[1],
                avlTime[0], avlTime[1],
                rbtTime[0], rbtTime[1]);

        elements = (int) (elements * 1.31);
    }
    fclose(output);


    // ONLY FOR RBT AND AVL, bst takes WAY TOO MUCH TIME
    // 10% of random elements
    elements = 100;
    output = fopen("second_part/times/finds_ordered_avl_rbt.txt", "w");
    fprintf(output, "N,T2,D2,T3,D3\n");
    printf("N T1 D2 T3 D3\n");
    for(int i = 0; i < 40; i++){

        printf("%d ", elements);

        initTime = getInitTimeOrdered(resolution, elements, seed);

        getAvlTimeFindOrdered(resolution, elements, seed, initTime, avlTime);
        printf("%.17g %.17g ", avlTime[0], avlTime[1]);

        getRbtTimeFindOrdered(resolution, elements, seed, initTime, rbtTime);
        printf("%.17g %.17g", rbtTime[0], rbtTime[1]);

        printf("\n");

        fprintf(output, "%d,%.17g,%.17g,%.17g,%.17g\n",
                elements,
                avlTime[0], avlTime[1],
                rbtTime[0], rbtTime[1]);

        elements = (int) (elements * 1.31);
    }
    fclose(output);
    */
}

