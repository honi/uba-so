#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int aprietes = 5;

void sigurg_handler(int sig){
	aprietes--;
	printf("ya va!\n");
}

void sigint_handler(int sig){
	wait(NULL);
	exit(0);
}

void funcionalidad_hijo(int argc, char* argv[]){
	signal(SIGURG,sigurg_handler);
	while(aprietes > 0);
	kill(getppid(),SIGINT);
	for(int i = 1; i < argc-1;i++){
		argv[i] = argv[i-1];
	}
	argv[argc-1] = 0;
	execv(argv[0],argv);
}


int main(int argc, char* argv[]) {
	signal(SIGINT,sigint_handler);
  	pid_t hijo = fork();
	if(hijo == 0){
		funcionalidad_hijo(argc,argv);
	}
	while(1){
		sleep(1);
		printf("sup!\n");
		kill(hijo,SIGURG);
	}
	return 0;
}
