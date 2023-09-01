#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int pipe_fd[2];
pid_t parent_pid;
pid_t child_pid;

void handle_sigint_child() {
    printf("Dejame pensarlo...\n");
    sleep(5);
    printf("Ya sé el significado de la vida.\n");
    char msg[2] = "42";
    write(pipe_fd[WRITE], &msg, sizeof(msg));
    kill(parent_pid, SIGINT);
}

void handle_sighup_child() {
    printf("Me voy a mirar crecer las flores desde abajo.\n");
    exit(EXIT_SUCCESS);
}

void handle_sigint_parent() {
    char msg[2];
    read(pipe_fd[READ], &msg, sizeof(msg));
    printf("Mirá vos. El significado de la vida es %s.\n", msg);
    printf("¡Bang Bang, estás liquidado!\n");
    kill(child_pid, SIGHUP);
    sleep(10);
}

void handle_sigchld_parent() {
    wait(NULL);
    printf("Te voy a buscar en la oscuridad.\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if (pipe(pipe_fd) < 0){
        fprintf(stderr, "pipe error\n");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();
    if (child_pid < 0) {
        fprintf(stderr, "fork error\n");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Child
        close(pipe_fd[READ]);
        parent_pid = getppid();
        signal(SIGINT, handle_sigint_child);
        signal(SIGHUP, handle_sighup_child);
    } else { // Parent
        close(pipe_fd[WRITE]);
        signal(SIGINT, handle_sigint_parent);
        signal(SIGCHLD, handle_sigchld_parent);
        sleep(1);
        printf("¿Cuál es el significado de la vida?\n");
        kill(child_pid, SIGINT);
    }

    while(1);
}
