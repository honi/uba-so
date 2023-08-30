#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int generate_random_number() {
	return rand() % 50;
}

void worker(int id, int controller, int pipe_read, int pipe_write, int pipe_parent) {
	int number = -1;
	int secret = 0;

	if (controller) {
		read(pipe_read, &number, sizeof(number));
		secret = number + generate_random_number();
		printf("[worker %d] Starting... Secret number is: %d\n", id, secret);
	}

	while(1) {
		if (controller || number >= 0) {
			printf("[worker %d] Sending number: %d\n", id, number);
			write(pipe_write, &number, sizeof(number));
		}

		read(pipe_read, &number, sizeof(number));

		if (controller && number >= secret) {
			printf("[worker %d] Secret number found!\n", id);
			write(pipe_parent, &number, sizeof(number));
			exit(EXIT_SUCCESS);
		}

		number++;
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));

	if (argc != 4) {
		fprintf(stderr, "Usage: %s <n> <start_number> <controller>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	int start_number = atoi(argv[2]);
	int controller = atoi(argv[3]);

	if (controller <= 0 || controller > n) {
		fprintf(stderr, "Controller must be between 1 and %d\n", n);
		exit(EXIT_FAILURE);
	}

	printf("N: %d\n", n);
	printf("Start number: %d\n", start_number);
	printf("Controller: %d\n", controller);

	int MASTER_PIPE = n;
	int pipes[n + 1][2];
	for (int i = 0; i < n + 1; i++) {
		if (pipe(pipes[i]) < 0) {
			fprintf(stderr, "Error creating pipe #%d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	pid_t children[n];
	for (int i = 0; i < n; i++) {
		children[i] = fork();

		if (children[i] < 0) {
			fprintf(stderr, "Error creating process #%d\n", i);
			exit(EXIT_FAILURE);
		}

		if (children[i] == 0) {
			int id = i + 1;
			worker(
				id,
				id == controller,
				pipes[i][PIPE_READ],
				pipes[(i + 1) % n][PIPE_WRITE],
				pipes[MASTER_PIPE][PIPE_WRITE]
			);
		}
	}

	// Iniciamos el juego escribiendo el start_number en el pipe del proceso
	// controlador. A partir de ahí empieza a circular el número por el anillo.
	write(pipes[controller - 1][PIPE_WRITE], &start_number, sizeof(start_number));

	// No cerramos ningún fd de los pipes porque siempre leemos y escribimos una
	// cantidad específica de bytes (nunca esperamos EOF), y si nuestro programa
	// funciona bien, en algún momento vamos a poder leer end_number del master pipe.
	// Ahí se va a desbloquear este read() y luego matamos forzosamente a todos los
	// procesos hijos con SIGKILL.

	int end_number;
	read(pipes[MASTER_PIPE][PIPE_READ], &end_number, sizeof(end_number));

	for (int i = 0; i < n; i++) {
		kill(children[i], SIGKILL);
	}

	printf("Finished with number: %d\n", end_number);
}
