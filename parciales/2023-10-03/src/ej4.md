## Ejercicio 4

### a)

```c
#define READ 0
#define WRITE 1

void child(int m, int fdw) {
    // Pedimos el número de labo y lo escribimos en el pipe para que el proceso padre
    // pueda leerlo más tarde.
    int labo = dameLabo(getpid(), m);
    write(fdw, &labo, sizeof(labo));
    exit(EXIT_SUCCESS);
}

void main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(STDERR_FILENO, "%s <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    // Creamos n pipes para comunicarnos con nuestros hijos.
    // Los pipes son unidireccionales, pero solo necesitamos 1 por cada hijo
    // para la comunicación hijo -> padre cuando el hijo informa el labo asignado.
    int pipes[n][2];
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) < 0) {
            fprintf(STDERR_FILENO, "error creando pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    // Creamos los n procesos hijos.
    pid_t children[n];
    for (int i = 0; i < n; i++) {
        children[i] = fork();
        if (children[i] < 0) {
            fprintf(STDERR_FILENO, "error creando proceso hijo\n");
            exit(EXIT_FAILURE);
        }
        if (children[i] == 0) {
            // Cerramos todos los pipes que no vamos a usar.
            // Esto no es realmente necesario porque en cada pipe vamos a escribir una
            // cantidad fija de bytes, entonces no necesitamos generar las condiciones
            // necesarias para que el SO nos retorne un EOF cuando hacemos read().
            for (int j = 0; j < n; j++) {
                close(pipes[j][READ]);
                if (j != i) close(pipes[j][WRITE]);
            }
            // Los procesos hijos heredan una copia de la memoria del padre, incluyendo
            // los file descriptors. En particular ya tenemos el valor de m en el proceso
            // hijo, no hace falta pasarlo por un pipe padre -> hijo.
            child(m, pipes[i][WRITE]);

            // Nunca volvemos a main() en los procesos hijos porque hacen un exit().
        }
    }

    // Ya creamos los n hijos que van a escribir sus labos en cada uno de sus pipes.
    // Leemos los labos asignados y lo imprimimos junto al pid del hijo.
    for (int i = 0; i < n; i++) {
        // Si bien leemos los pipes en orden (0, 1, ..., n-1), no tenemos garantías de que
        // los resultados esten disponibles en ese orden. No obstante, si el hijo aún no
        // escribió su labo en el pipe, el llamado a read() va a bloquear al padre hasta
        // que haya sizeof(int) bytes disponibles en el pipe. Esto siempre va a suceder por
        // construcción del programa (salvo errores fatales por parte del SO), por eso el
        // padre eventualmente va a terminar.
        int labo;
        read(pipes[i][READ], &labo, sizeof(labo));
        printf("%d: %d\n", children[i], labo);

        // Hacemos wait por alguno de los hijos que terminó para evitar que quede zombie.
        // No hay garantías que el pid retornado sea el del hijo i-ésimo, pero no nos
        // importa eso, sino simplemente queremos hacer en total n wait().
        wait(NULL);
    }

    // Podríamos acá cerrar los pipes, pero como el proceso padre ya termina, el SO se
    // va a encargar de hacer eso automáticamente.

    exit(EXIT_SUCCESS);
}
```

### b)

Solo se modifica la última parte de `main()`. Omití los comentarios repetidos para ahorrar espacio.

```c
#define READ 0
#define WRITE 1

void child(int m, int fdw) {
    int labo = dameLabo(getpid(), m);
    write(fdw, &labo, sizeof(labo));
    exit(EXIT_SUCCESS);
}

void main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(STDERR_FILENO, "%s <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int pipes[n][2];
    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) < 0) {
            fprintf(STDERR_FILENO, "error creando pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    pid_t children[n];
    for (int i = 0; i < n; i++) {
        children[i] = fork();
        if (children[i] < 0) {
            fprintf(STDERR_FILENO, "error creando proceso hijo\n");
            exit(EXIT_FAILURE);
        }
        if (children[i] == 0) {
            for (int j = 0; j < n; j++) {
                close(pipes[j][READ]);
                if (j != i) close(pipes[j][WRITE]);
            }
            child(m, pipes[i][WRITE]);
        }
    }

    // Abrimos el archivo para escritura y reconfiguramos nuestro file descriptor 1 (stdout)
    // para que apunte al archivo abierto. De esta forma, cuando nuevaSalida escribe en
    // stdout, en realidad va a estar escribiendo en el archivo. Recordemos que los fd
    // son heredados por los hijos.
    int fd_out = open("resultados.out", O_WRONLY);
    dup2(fd_out, STDOUT_FILENO);

    for (int i = 0; i < n; i++) {
        // Leemos el labo asignado al hijo i-ésimo.
        int labo;
        read(pipes[i][READ], &labo, sizeof(labo));
        wait(NULL);

        // Creamos un pipe para stdin.
        int fd_in[2];
        if (pipe(fd_in) < 0) {
            fprintf(STDERR_FILENO, "error creando pipe\n");
            exit(EXIT_FAILURE);
        }

        // Creamos el proceso hijo para ejecutar el programa nuevaSalida.
        pid_t nuevaSalida = fork();
        if (nuevaSalida < 0) {
            fprintf(STDERR_FILENO, "error creando proceso hijo\n");
            exit(EXIT_FAILURE);
        }

        if (nuevaSalida == 0) {
            // De forma similar al stdout, reconfiguramos el file descriptor 0 (stdin)
            // para que apunte a este pipe compartido entre padre e hijo. De esta forma
            // podemos desde el padre escribir al pipe y que el hijo lo lea por stdin.
            dup2(fd_in[READ], STDIN_FILENO);

            // Desde el hijo no vamos a escribir en nuestro propio stdin. Pero cerramos
            // el pipe principalmente para que nuevaSalida pueda recibir un EOF cuando
            // cerramos la punta de escritura del pipe desde el padre.
            close(fd_in[WRITE]);

            // Reemplazamos el proceso hijo por la ejecución del programa nuevaSalida.
            // Se mantienen los file descriptors prevamiente configurados.
            char* argv_nuevaSalida = {"nuevaSalida", NULL};
            execvp(argv_nuevaSalida[0], argv_nuevaSalida);

            // Nunca llegamos acá si execvp funcionó correctamente.
        } else {
            // Somos el padre, escribimos en el pipe de stdin el pid del proceso hijo
            // (el que representa un estudiante) junto a su labo asignado.
            fprintf(fd_in[WRITE], "%d %d\n", children[i], labo);

            // Cerramos el pipe.
            close(fd_in[READ]);
            close(fd_in[WRITE]); // Este es el que importa para disparar EOF al hijo.

            // Esperamos que termine nuevaSalida antes de continuar.
            waitpid(nuevaSalida, NULL, 0);
        }
    }

    exit(EXIT_SUCCESS);
}
```
