# Ejercicio 1

```c
#define READ 0
#define WRITE 1
#define FROM_PARENT 0
#define TO_PARENT 1

void verificador(int fdr, int fdw) {
    int num;
    int count = 0;
    while (read(fdr, &num, sizeof(num))) {
        if (esPerfecto(num)) count++;
    }
    write(fdw, &count, sizeof(count));
    exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
    int n = atoi(argv[1]);

    int pipes[n][2][2];
    for (int i = 0; i < n; i++) {
        pipe(pipes[i][FROM_PARENT]);
        pipe(pipes[i][TO_PARENT]);
    }

    pid_t children[n];
    for (int i = 0; i < n; i++) {
        if ((children[i] = fork()) == 0) { // child
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    close(pipe[j][FROM_PARENT][READ]);
                    close(pipe[j][FROM_PARENT][WRITE]);
                    close(pipe[j][TO_PARENT][READ]);
                    close(pipe[j][TO_PARENT][WRITE]);
                } else {
                    close(pipe[j][FROM_PARENT][WRITE]);
                    close(pipe[j][TO_PARENT][READ]);
                }
            }
            verificador(pipes[i][FROM_PARENT][READ], pipes[TO_PARENT][WRITE]);
        }
    }

    int child = 0;
    while (hayNumero()) {
        int num = leerNumero();
        write(pipes[child][FROM_PARENT][WRITE], &num, sizeof(num));
        child = (child+1) % n;
    }

    for (int i = 0; i < n; i++) {
        close(pipes[i][FROM_PARENT][READ]);
        close(pipes[i][FROM_PARENT][WRITE]);
    }

    int totales[n];
    int num;
    for (int i = 0; i < n; i++) {
        totales[n] = read(pipes[i][TO_PARENT][READ], &num, sizeof(num));
        wait(NULL); // Evitamos dejar procesos zombies.
    }

    int fd_informe = open("informe.txt", O_WRONLY | O_APPEND);

    int fd_startgpt_stdin[2];
    pipe(fd_startgpt_stdin);

    for (int i = 0; i < n; i++) {
        dprintf(fd_startgpt_stdin[WRITE], "%d\n", totales[i]);
    }
    close(fd_startgpt_stdin[WRITE]);

    dup2(fd_startgpt_stdin[READ], STDIN_FILENO);
    dup2(fd_informe, STDOUT_FILENO);

    char* argv[] = {"stargpt", "-generar-parrafo", NULL};
    execvp(argv[0], argv);
}
```
