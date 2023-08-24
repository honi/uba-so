#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid > 0) { // Parent
        // Don't wait() on the child process so it becomes a zombie.
        sleep(10);
    } else { // Child
        exit(EXIT_SUCCESS);
    }

    return 0;
}
