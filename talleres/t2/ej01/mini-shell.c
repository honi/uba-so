#include "constants.h"
#include "mini-shell-parser.c"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int run(pid_t *children, char ***progs, size_t count) {
	int pipe_stdin[2];
	int pipe_stdout[2];

	for (int i = 0; i < count; i++) {
		// Si no es el último proceso, creamos un pipe para stdout y así conectar éste
		// proceso con el siguiente. Tenemos que crear el pipe antes de forkear para
		// que el proceso padre mantenga ésta referencia y luego conectar el fd de
		// lectura de éste pipe con el stdin del siguiente proceso.
		if (i < count - 1) {
			if (pipe(pipe_stdout) < 0) {
				fprintf(stderr, "Error creating pipe #%d\n", i);
				return EXIT_FAILURE;
			}
		}

		children[i] = fork();

		if (children[i] < 0) {
			fprintf(stderr, "Error creating process #%d\n", i);
			return EXIT_FAILURE;
		}

		if (children[i] == 0) {
			// Si no es el primer proceso, lo conectamos con el anterior usando el pipe
			// para stdin. Importante cerrar el fd de escritura así éste proceso recibe
			// EOF cuando el proceso anterior termina y cierra su fd. No todos los
			// programas requieren esto, en esencia es necesario solo para los programas
			// que hacen algo como while(read(stdin)) {...}
			if (i > 0) {
				dup2(pipe_stdin[PIPE_READ], STDIN_FILENO);
				close(pipe_stdin[PIPE_WRITE]);
			}

			// Si no es el último proceso, conectamos su stdout al pipe que creamos antes.
			if (i < count - 1) {
				dup2(pipe_stdout[PIPE_WRITE], STDOUT_FILENO);
				close(pipe_stdout[PIPE_READ]);
			}

			if (execvp(progs[i][0], progs[i]) < 0) {
				fprintf(stderr, "Error calling execvp for program: %s\n", progs[i][0]);
				return EXIT_FAILURE;
			}
		} else {
			// Rotamos los pipes. El que usamos para stdout en ésta iteración lo vamos
			// a usar para stdin en la siguiente. A su vez, como acá estamos en el
			// proceso padre, vamos cerrando los fd para permitir que el SO genere el
			// EOF cuando corresponda.
			if (i > 0) {
				close(pipe_stdin[PIPE_READ]);
				close(pipe_stdin[PIPE_WRITE]);
			}
			pipe_stdin[PIPE_READ] = pipe_stdout[PIPE_READ];
			pipe_stdin[PIPE_WRITE] = pipe_stdout[PIPE_WRITE];
		}
	}

	int status;
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);
		status = WEXITSTATUS(status);
		if (status != 0) {
			fprintf(stderr, "Process %d failed\n", children[i]);
			return status;
		}
	}

	return EXIT_SUCCESS;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s 'ls -a | grep mini'\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	int children_size = sizeof(void*) * programs_count;
	pid_t* children = malloc(children_size);
	memset(children, 0, children_size);

	int status = run(children, programs_with_parameters, programs_count);
	printf("Status: %d\n", status);

	// Si hubo un error matamos a todos los procesos hijos para ser un buen programa.
	if (status != 0) {
		for (int i = 0; i < programs_count; i++) {
			if (children[i] > 0) kill(children[i], SIGKILL);
		}
	}

	fflush(stdout);
	fflush(stderr);
	free(children);

	exit(EXIT_SUCCESS);
}
