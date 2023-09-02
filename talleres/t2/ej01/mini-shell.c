#include "constants.h"
#include "mini-shell-parser.c"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void run(char ***progs, size_t count) {
	pid_t children[count];
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
				exit(EXIT_FAILURE);
			}
		}

		children[i] = fork();

		if (children[i] < 0) {
			fprintf(stderr, "Error creating process #%d\n", i);
			exit(EXIT_FAILURE);
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
				exit(EXIT_FAILURE);
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

	// Esperamos que terminen todos los procesos y recolectamos su status.
	int status[count];
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status[i], 0);
	}

	// Revisamos cómo terminaron los procesos e imprimimos mensajes de error de ser necesario.
	for (int i = 0; i < count; i++) {
		int exit_status = WEXITSTATUS(status[i]);
		if (exit_status != 0) {
			fprintf(stderr, "Command %s terminated abnormally with status: %d\n", progs[i][0], exit_status);
		}
		// La macro WIFSIGNALED nos dice si el proceso terminó debido a una señal que no fue manejada.
		// https://www.gnu.org/software/libc/manual/html_node/Process-Completion-Status.html#index-WIFSIGNALED
		if (WIFSIGNALED(status[i]) != 0) {
			int exit_signal = WTERMSIG(status[i]);
			fprintf(stderr, "Command %s terminated because of signal: %d - %s\n", progs[i][0], exit_signal, strsignal(exit_signal));
		}
	}
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s 'ls -a | grep mini'\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	run(programs_with_parameters, programs_count);

	fflush(stdout);
	fflush(stderr);

	return EXIT_SUCCESS;
}
