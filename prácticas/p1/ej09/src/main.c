#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pid_t ping_pid;
pid_t pong_pid;
int count = 0;

void ping() {
    // Every 3rd ping...
    if (count++ % 3 == 0 && count != 1) {
        char answer = 0;
        do {
            printf("Continue [y/n]? ");
            answer = tolower(getchar());
            while (getchar() != '\n');
        } while (answer != 'y' & answer != 'n');
        if (answer == 'n') {
            kill(pong_pid, SIGKILL);
            exit(EXIT_SUCCESS);
        }
    }

    printf("[%d] ping\n", ping_pid);
    kill(pong_pid, SIGUSR1);
}

void pong() {
    printf("[%d] pong\n", pong_pid);
    kill(ping_pid, SIGUSR1);
}

int main(int argc, char const *argv[]){
    ping_pid = getpid();
    pong_pid = fork();

    if (pong_pid < 0) exit(EXIT_FAILURE);

    if (pong_pid > 0) {
        signal(SIGUSR1, ping);
        ping();
    } else {
        pong_pid = getpid();
        signal(SIGUSR1, pong);
    }

    while (1);
}
