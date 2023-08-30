#include "sort.c"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define PARENT_TO_CHILD 0
#define CHILD_TO_PARENT 1

void worker(int id, int M, int pipe_fd[2][2]) {
    close(pipe_fd[PARENT_TO_CHILD][WRITE]);
    close(pipe_fd[CHILD_TO_PARENT][READ]);

    int row[M];
    while (read(pipe_fd[PARENT_TO_CHILD][READ], &row, sizeof(int) * M)) {
        printf("Worker #%d received row\n", id + 1);
        int result = 0;
        for (int i = 0; i < M; i++) result += row[i];
        result /= M;
        write(pipe_fd[CHILD_TO_PARENT][WRITE], &result, sizeof(result));
    }

    printf("Worker #%d finished\n", id + 1);
    exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
    if (argc != 2) {
        printf("Missing matrix file name. Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");

    int N, M;
    fscanf(fp, "%d %d", &N, &M);

    int matrix[N][M];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }

    fclose(fp);
    printf("Matrix size: %d %d\n", N, M);

    const int WORKERS = get_nprocs_conf();
    printf("Workers: %d\n", WORKERS);

    printf("Creating pipes...\n");
    int pipes[WORKERS][2][2];
    for (int i = 0; i < WORKERS; i++) {
        pipe(pipes[i][PARENT_TO_CHILD]); // parent -> child
        pipe(pipes[i][CHILD_TO_PARENT]); // parent <- child
    }

    printf("Creating workers...\n");
    pid_t children[WORKERS];
    for (int i = 0; i < WORKERS; i++) {
        children[i] = fork();

        if (children[i] < 0) {
            printf("Error creating worker #%d\n", i + 1);
            while(i-- >= 0) kill(children[i], SIGKILL);
            exit(EXIT_FAILURE);
        }

        if (children[i] == 0) {
            // Como todos los hijos heredan todos los file descriptors, tenemos que
            // cerrar los pipes de los otros hijos para que funcione correctamente
            // la detección del EOF del pipe.
            for (int j = 0; j < WORKERS; j++) {
                if (j != i) {
                    close(pipes[j][PARENT_TO_CHILD][READ]);
                    close(pipes[j][PARENT_TO_CHILD][WRITE]);
                    close(pipes[j][CHILD_TO_PARENT][READ]);
                    close(pipes[j][CHILD_TO_PARENT][WRITE]);
                }
            }

            worker(i, M, pipes[i]);
        }
    }

    printf("Sending rows to workers...\n");
    int worker = 0;
    for (int i = 0; i < N; i++) {
        write(pipes[worker][PARENT_TO_CHILD][WRITE], &matrix[i], sizeof(int) * M);
        worker = ++worker % WORKERS; // round-robin
    }

    printf("Closing file descriptors from parent to child...\n");
    for (int i = 0; i < WORKERS; i++) {
        // Al cerrar estos file descriptors vamos a interrumpir el while(read()) que
        // está corriendo en todos los hijos, forzando que terminen.
        close(pipes[i][PARENT_TO_CHILD][WRITE]);

        // Al cerrar estos files descriptors vamos a poder hacer while(read()) desde el
        // proceso padre para consumir todo lo que hayan escrito los hijos.
        close(pipes[i][CHILD_TO_PARENT][WRITE]);
    }

    // Leemos los resultados de los hijos en orden por número de worker. No hay ninguna
    // garantía que el worker 0 haya terminado su trabajo antes que el worker 1, 2, etc.
    // No obstante, como todos los hijos van a terminar, el padre eventualmente va a
    // desbloquearse en el read() y continuar con el resto de los hijos.

    // Una arquitectura mejor sería que los procesos hijos notifiquen de alguna forma
    // al padre que terminaron de procesar, y recién ahí leemos del pipe. Así podemos
    // ir recolectando los resultados a medida que están disponibles sin importar qué
    // hijo terminó primero.

    printf("Reading results from workers...\n");
    worker = 0;
    int results[N];
    int* r = results;
    for (int i = 0; i < WORKERS; i++) {
        while (read(pipes[i][CHILD_TO_PARENT][READ], r, sizeof(int))) r++;
    }

    sort(results, N);
    printf("Results:\n");
    for (int i = 0; i < N; i++) {
        printf("%d", results[i]);
        printf(i == N - 1 ? "\n" : " ");
    }

    return EXIT_SUCCESS;
}
