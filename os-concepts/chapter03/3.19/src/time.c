#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const char* SHM_NAME = "/time_shared_memory";
const int SHM_SIZE = sizeof(struct timeval);

struct timeval* start_time;
struct timeval end_time;

unsigned long get_timestamp(struct timeval* t) {
    return 1000000 * t->tv_sec + t->tv_usec;
}

void run_command(char *const argv[]) {
    // Track execution start time.
    gettimeofday(start_time, NULL);

    // Execute command we want to time.
    execvp(argv[0], argv);

    // If we execute these lines then execvp failed.
    fprintf(stderr, "execvp error\n");
    exit(EXIT_FAILURE);
}

void handle_sigchld() {
    // Avoid zombie process and get child exit status.
    int status;
    wait(&status);

    // Calculate elapsed time.
    gettimeofday(&end_time, NULL);
    unsigned long time = get_timestamp(&end_time) - get_timestamp(start_time);
    printf("Elapsed time: %ld us\n", time);

    // Clean up.
    shm_unlink(SHM_NAME);
    munmap(start_time, SHM_SIZE);

    // Forward child exit status.
    exit(WEXITSTATUS(status));
}

int main(int argc, char *const argv[]) {
    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        fprintf(stderr, "shm_open error\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, SHM_SIZE) < 0) {
        fprintf(stderr, "ftruncate error\n");
        exit(EXIT_FAILURE);
    }

    start_time = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (start_time < 0) {
        fprintf(stderr, "mmap error\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, handle_sigchld);

    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid > 0) { // Parent
        while (1);
    } else { // Child
        run_command(++argv);
    }
}
