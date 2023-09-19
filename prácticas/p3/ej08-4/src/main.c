#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A 0
#define B 1
#define C 2

sem_t b_empty;
sem_t b_full;
sem_t c_empty;
sem_t c_full;

void* worker_A() {
    int consumer = B;
    while (1) {
        printf("\nA");
        if (consumer == B) {
            sem_post(&b_full);
            sem_wait(&b_empty);
            consumer = C;
        } else {
            sem_post(&c_full);
            sem_post(&c_full);
            sem_wait(&c_empty);
            sem_wait(&c_empty);
            consumer = B;
        }
    }
}

void* worker_B() {
    while (1) {
        sem_wait(&b_full);
        printf("B");
        sem_post(&b_empty);
    }
}

void* worker_C() {
    while (1) {
        sem_wait(&c_full);
        printf("C");
        sem_post(&c_empty);
    }
}

int main(int argc, char* argv[]) {
    sem_init(&b_empty, 0, 0);
    sem_init(&b_full, 0, 0);
    sem_init(&c_empty, 0, 0);
    sem_init(&c_full, 0, 0);

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
