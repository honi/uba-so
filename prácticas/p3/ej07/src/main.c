#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
#define start 4

sem_t s[N];

void* work(void* arg) {
    int i = *(int*)arg;
    sem_wait(&s[i]);
    printf("%d\n", i);
    sem_post(&s[(i+1) % N]);
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_t workers[N];
    int ids[N];
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        sem_init(&s[i], 0, 0);
        pthread_create(&workers[i], &attr, work, &ids[i]);
    }

    sem_post(&s[start]);

    for (int i = 0; i < N; i++) {
        pthread_join(workers[i], NULL);
    }

    return EXIT_SUCCESS;
}
