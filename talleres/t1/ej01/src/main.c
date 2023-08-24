#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

#define MAX_N 10
#define SLEEP_TIME 1
#define SYNC_TIME 1

int N;
int K;
int J;
pid_t children[MAX_N];

void child_handler(int sig) {
    pid_t pid = getpid();
    int r = (int) time(NULL) % N;
    printf("[%d] r = %d\n", pid, r);
    if (r == J) {
        printf("[%d] I'm dead :(\n", pid);
        exit(EXIT_SUCCESS);
    }
}

void init_child(int seed) {
    signal(SIGTERM, child_handler);
    while (true) {
        sleep(SYNC_TIME);
    }
}

void create_children() {
    for (int c = 0; c < N; c++) {
        children[c] = fork();
        if (children[c] < 0) {
            fprintf(stderr, "Fork error\n");
            exit(EXIT_FAILURE);
        } else if (children[c] == 0) {
            init_child(children[c]);
        } else {
            printf("[parent] Created child %d\n", children[c]);
        }
    }
    sleep(SYNC_TIME);
}

void parent_handler(int sig) {
    pid_t child_pid = wait(NULL);
    printf("[parent] SIGCHLD -> %d\n", child_pid);
    for (int c = 0; c < N; c++) {
        if (children[c] == child_pid) {
            children[c] = 0;
            return;
        }
    }
}

void run_round() {
    for (int c = 0; c < N; c++) {
        if (children[c] == 0) continue;
        printf("[parent] SIGTERM -> %d\n", children[c]);
        kill(children[c], SIGTERM);
        sleep(SLEEP_TIME);
    }
}

void check_survivors() {
    printf("[parent] Checking for survivors...\n");
    int count = 0;
    for (int c = 0; c < N; c++) {
        if (children[c] != 0) {
            printf("[parent] Child #%d (PID %d) has survived\n", c, children[c]);
            kill(children[c], SIGKILL);
            count++;
        }
    }
    if (count == 0) {
        printf("[parent] Everybody died :(\n");
    }
}

int main(int argc, char const *argv[]) {
    // TODO: Validar argumentos de entrada.
    N = atoi(argv[1]);
    K = atoi(argv[2]);
    J = atoi(argv[3]);

    printf("N = %d\nK = %d\nJ = %d\n", N, K, J);

    create_children();

    signal(SIGCHLD, parent_handler);
    while (K-- > 0) run_round();
    signal(SIGCHLD, SIG_DFL);

    check_survivors();

    exit(EXIT_SUCCESS);
}
