#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A 0
#define B 1
#define C 2

sem_t s[3];

void* worker_A() {
    while (1) {
        sem_wait(&s[A]);
        printf("\nA");
        sem_post(&s[B]);
    }
}

void* worker_B() {
    while (1) {
        sem_wait(&s[B]);
        printf("B");
        sem_post(&s[C]);
    }
}

void* worker_C() {
    while (1) {
        sem_wait(&s[C]);
        printf("C");
        sem_post(&s[A]);
    }
}

int main(int argc, char* argv[]) {
    sem_init(&s[A], 0, 1);
    sem_init(&s[B], 0, 0);
    sem_init(&s[C], 0, 0);

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
