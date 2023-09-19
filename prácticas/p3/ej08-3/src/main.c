#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A 0
#define B 1
#define C 2

sem_t empty;
sem_t full;

void* worker_A() {
    while (1) {
        sem_wait(&empty);
        sem_wait(&empty);
        printf("\nA");
        sem_post(&full);
        sem_post(&full);
    }
}

void* worker_B() {
    while (1) {
        sem_wait(&full);
        printf("B");
        sem_post(&empty);
    }
}

void* worker_C() {
    while (1) {
        sem_wait(&full);
        printf("C");
        sem_post(&empty);
    }
}

int main(int argc, char* argv[]) {
    sem_init(&empty, 0, 2);
    sem_init(&full, 0, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_t workers[3];
    pthread_create(&workers[A], &attr, worker_A, NULL);
    pthread_create(&workers[B], &attr, worker_B, NULL);
    pthread_create(&workers[C], &attr, worker_C, NULL);

    for (int i = 0; i < 3; i++) {
        pthread_join(workers[i], NULL);
    }

    return EXIT_SUCCESS;
}
