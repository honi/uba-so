#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define PARENT_TO_CHILD 0
#define CHILD_TO_PARENT 1

#define RANGE_MIN 2L
#define RANGE_MAX 1000000001L

void worker(int id, int pipe_fd[2][2]) {
    long start = 0;
    long end = 0;
    read(pipe_fd[PARENT_TO_CHILD][READ], &start, sizeof(start));
    read(pipe_fd[PARENT_TO_CHILD][READ], &end, sizeof(end));
    printf("Worker #%d received sub range\n", id + 1);

    long result = 0;
    for (int i = start; i <= end; i++) {
        if (i % 2 == 0) result++;
    }

    write(pipe_fd[CHILD_TO_PARENT][WRITE], &result, sizeof(result));
    close(pipe_fd[CHILD_TO_PARENT][WRITE]);

    exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
    if (argc != 2) {
        printf("Missing process count. Usage: %s <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    printf("Workers: %d\n", N);

    printf("Creating pipes...\n");
    int pipes[N][2][2];
    for (int i = 0; i < N; i++) {
        pipe(pipes[i][PARENT_TO_CHILD]); // parent -> child
        pipe(pipes[i][CHILD_TO_PARENT]); // parent <- child
    }

    printf("Creating workers...\n");
    pid_t children[N];
    for (int i = 0; i < N; i++) {
        children[i] = fork();

        if (children[i] < 0) {
            printf("Error creating worker #%d\n", i + 1);
            while(i-- >= 0) kill(children[i], SIGKILL);
            exit(EXIT_FAILURE);
        }

        if (children[i] == 0) {
            worker(i, pipes[i]);
        }
    }

    long step = (RANGE_MAX - RANGE_MIN) / N;
    printf("Sending sub ranges to workers...\n");
    printf("Range: [%ld, %ld]\n", RANGE_MIN, RANGE_MAX);
    printf("Step: %ld\n", step);

    for (int i = 0; i < N; i++) {
        long start = RANGE_MIN + step * i;
        long end = i == N - 1 ? RANGE_MAX : RANGE_MIN + step * (i + 1) - 1;
        printf("Sending sub range to worker #%d: [%ld, %ld]\n", i + 1, start, end);
        write(pipes[i][PARENT_TO_CHILD][WRITE], &start, sizeof(start));
        write(pipes[i][PARENT_TO_CHILD][WRITE], &end, sizeof(end));
    }

    printf("Reading results from workers...\n");
    long result = 0;
    for (int i = 0; i < N; i++) {
        long worker_result = 0;
        read(pipes[i][CHILD_TO_PARENT][READ], &worker_result, sizeof(worker_result));
        printf("Received result from worker #%d: %ld\n", i + 1, worker_result);
        result += worker_result;
    }

    printf("Result: %ld\n", result);

    // Evitamos los procesos zombies.
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // No hace falta cerrar explícitamente los pipes porque estamos leyendo una
    // cantidad fija de bytes, y nuestro programa (si funciona bien) escribe
    // exactamente esa misma cantidad.
    // Al terminar los procesos el SO cierra los file descriptors automáticamente.

    // Casos borde no manejados:
    // - Si el padre muere a mitad de camino nunca mata a los hijos.

    return EXIT_SUCCESS;
}
