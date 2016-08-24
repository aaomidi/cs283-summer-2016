#include <stdio.h>
#include "csapp.h"

#define PEOPLE_COUNT 100
#define GETUP_COUNT 100000
#define TRIALS 10
volatile long globalCounter = 0;
pthread_mutex_t *lock;

/**
 * Does not use locks.
 * @param arg
 * @return
 */
void *globalIncrement(void *arg) {
    int i;
    for (i = 0; i < GETUP_COUNT; i++) {
        globalCounter++;
    }
    return NULL;
}

/**
 * Increments inside loop but locks outside.
 * @param arg
 * @return
 */
void *lockIncrementPlus(void *arg) {
    int i;
    pthread_mutex_lock(lock);
    for (i = 0; i < GETUP_COUNT; i++) {
        globalCounter++;
    }
    pthread_mutex_unlock(lock);

    return NULL;
}

/**
 * Locks and increments inside loop
 * @param arg
 * @return
 */
void *lockIncrement(void *arg) {
    int i;
    for (i = 0; i < GETUP_COUNT; i++) {
        pthread_mutex_lock(lock);
        globalCounter++;
        pthread_mutex_unlock(lock);
    }

    return NULL;
}

/**
 * Runs the pthreads and calls the routine.
 * @param routine
 */
void decide(void *(*routine)(void *)) {
    int x = 0;
    clock_t start, stop;
    while (x++ < TRIALS) {
        globalCounter = 0;
        lock = Malloc(sizeof(pthread_mutex_t));
        pthread_t threads[PEOPLE_COUNT];

        if (pthread_mutex_init(lock, NULL)) {
            fprintf(stderr, "Mutex wasn't initialized.\n");
            exit(1);
        }

        int i;
        start = clock();
        for (i = 0; i < PEOPLE_COUNT; i++) {
            Pthread_create(&threads[i], NULL, routine, NULL);
        }

        for (i = 0; i < PEOPLE_COUNT; i++) {
            pthread_join(threads[i], NULL);
        }
        stop = clock();

        pthread_mutex_destroy(lock);
        free(lock);
        printf("Run %d:\n\tGlobal variable size: %ld (%i)\n\tTime taken: %ld seconds\n", x, globalCounter,
               PEOPLE_COUNT * GETUP_COUNT, (stop - start) / CLOCKS_PER_SEC);
    }
}

int main(int argc, char **argv) {
    int option = -1;
    if (argc == 2) {
        option = atoi(argv[1]);
    }
    switch (option) {
        case 1:
            printf("Starting no locks:\n");
            decide(globalIncrement);
            break;
        case 2:
            printf("\nStarting with locks\n");
            decide(lockIncrement);
            break;
        case 3:
            printf("\nStarting with better locks\n");
            decide(lockIncrementPlus);
            break;
        default:
            printf("Starting no locks:\n");
            decide(globalIncrement);

            printf("\nStarting with locks\n");
            decide(lockIncrement);

            printf("\nStarting with better locks\n");
            decide(lockIncrementPlus);
            break;
    }
    exit(0);
}

