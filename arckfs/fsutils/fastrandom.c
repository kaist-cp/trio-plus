#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

static __thread uint32_t FAST_RANDOM_NEXT = 1;

#define NTHREADS 16

void fast_random_set_seed(uint32_t seed) {
    FAST_RANDOM_NEXT = seed;
}

uint32_t fast_random() {
    uint32_t new_val = FAST_RANDOM_NEXT * 1103515245 + 12345;
    FAST_RANDOM_NEXT = new_val;
    return (new_val / 65536) % 32768;
}

void* worker_thread(void* arg) {
    int tid = *(int*)arg;
    fast_random_set_seed(time(NULL) ^ tid);

    for (int i = 0; i < 5; i++) {
        printf("%d: Random number: %u\n", tid, fast_random());
    }

    return NULL;
}

int main() {
    pthread_t threads[NTHREADS];
    int tids[NTHREADS];

    for (int i = 0; i < NTHREADS; ++i) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, worker_thread, &tids[i]);
    }

    for (int i = 0; i < NTHREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}