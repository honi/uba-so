#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

char result_ready = 0;

void calculate(int number, int result_pipe[2]) {
    printf("Calculating for number %d: start\n", number);
    sleep((number + time(NULL)) % 5); // Simulamos un cómputo intensivo...
    int result = number + 666;
    write(result_pipe[WRITE], &result, sizeof(result));
    printf("Calculating for number %d: done\n", number);
    exit(EXIT_SUCCESS);
}

void handle_sigchld() {
    pid_t pid = wait(NULL);
    printf("SIGCHLD received (pid=%d)\n", pid);
    result_ready = 1;
}

void run_child(int id, int pipe_from_parent[2], int pipe_to_parent[2]) {
    int number;
    read(pipe_from_parent[READ], &number, sizeof(number));
    printf("Child #%d received number: %d\n", id, number);

    int result_pipe[2];
    pipe(result_pipe);
    signal(SIGCHLD, handle_sigchld);

    if (fork() == 0) { // TODO: Chequear errores...
        calculate(number, result_pipe);
    }

    char ping;
    int result;
    while (1) {
        read(pipe_from_parent[READ], &ping, sizeof(ping));
        write(pipe_to_parent[WRITE], &result_ready, sizeof(result_ready));

        if (result_ready) {
            printf("Result ready for child #%d\n", id);
            read(result_pipe[READ], &result, sizeof(result));
            write(pipe_to_parent[WRITE], &number, sizeof(number));
            write(pipe_to_parent[WRITE], &result, sizeof(result));
            exit(EXIT_SUCCESS);
        }
    }
}

void print_result(int child, int number, int result) {
    printf("Received result from child #%d: number = %d, result = %d\n", child, number, result);
}

int get_number(pid_t pid) {
    // Este número que devolvemos es arbitrario, podría ser cualquier cosa.
    return pid;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);

    int pipes[2 * N][2];
    for(int i = 0; i < 2 * N; i++) {
        pipe(pipes[i]);
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            run_child(i, pipes[i], pipes[N + i]);
        } else {
            int number = get_number(pid);
            printf("Sending number %d to child #%d\n", number, i);
            write(pipes[i][WRITE], &number, sizeof(number));
        }
    }

    char terminated[N];
    for (int i = 0; i < N; i++) terminated[i] = 0;

    int numTerminated = 0;
    while (numTerminated < N) {
        for (int i = 0; i < N; i++) {
            if (terminated[i]) continue;

            // El valor que escribimos es irrelevante, es solamente para desbloquear
            // el read() en el hijo.
            char ping = 42;
            write(pipes[i][WRITE], &ping, sizeof(ping));

            // Si el hijo nos responde con algo > 0 significa que terminó el cálculo.
            char child_result_ready = 0;
            read(pipes[N + i][READ], &child_result_ready, sizeof(child_result_ready));
            if (child_result_ready) {
                int number;
                int result;
                read(pipes[N + i][READ], &number, sizeof(number));
                read(pipes[N + i][READ], &result, sizeof(result));
                print_result(i, number, result);
                terminated[i] = 1;
                numTerminated++;
            }
        }
    }

    return EXIT_SUCCESS;
}
