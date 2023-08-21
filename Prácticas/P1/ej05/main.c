#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bart() {
	printf("Bart\n");
	exit(0);
}

void lisa() {
	printf("Lisa\n");
	exit(0);
}

void maggie() {
	printf("Maggie\n");
	exit(0);
}

void homer() {
	printf("Homer\n");

	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0) {
		exit(1);
	} else if (pid == 0) {
		return bart();
	} else {
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0) exit(1);
	}

	pid = fork();
	if (pid < 0) {
		exit(1);
	} else if (pid == 0) {
		return lisa();
	} else {
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0) exit(1);
	}

	pid = fork();
	if (pid < 0) {
		exit(1);
	} else if (pid == 0) {
		return maggie();
	} else {
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0) exit(1);
	}

	exit(0);
}

void abraham() {
	printf("Abraham\n");

	pid_t pid = fork();
	if (pid < 0) {
		exit(1);
	} else if (pid == 0) {
		return homer();
	} else {
		int status;
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0) exit(1);
		exit(0);
	}
}

int main() {
	// Abraham es padre de Homer
	// Homer es padre de Bart
	// Homer es padre de Lisa
	// Homer es padre de Maggie
	abraham();
	return 0;
}
