#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

void talk(int fd[2], pid_t self, pid_t other, bool send) {
    printf("%d\n", self);
    int next_msg = 0;
    while (next_msg < 10) {
        if (send) {
            printf("[%d] sending %d\n", self, next_msg);
            write(fd[WRITE], &next_msg, sizeof(next_msg));
            next_msg++;
        } else {
            send = true;
        }
        int received_msg;
        read(fd[READ], &received_msg, sizeof(received_msg));
        printf("[%d] received %d\n", self, received_msg);
    }
}

int main() {
    // Create pipe for IPC.
    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(stderr, "failed to create pipe");
        exit(EXIT_FAILURE);
    }

    // Create child process with fork.
    pid_t parent_pid = getpid();
    pid_t child_pid = fork();
    if (child_pid < 0) {
        fprintf(stderr, "failed to fork child process");
        exit(EXIT_FAILURE);
    }

    // Start communication.
    if (child_pid > 0) {
        // Parent
        talk(fd, parent_pid, child_pid, true);
    } else {
        // Child
        child_pid = getpid();
        talk(fd, child_pid, parent_pid, false);
    }

    close(fd[READ]);
    close(fd[WRITE]);
    exit(EXIT_SUCCESS);
}
