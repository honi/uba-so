#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10

atomic_int count = 0;

void* work(void* _) {
    printf("ready\n");
    count++;
    while (count < NUM_THREADS);
    printf("critical\n");
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_t workers[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&workers[i], &attr, work, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    return EXIT_SUCCESS;
}
