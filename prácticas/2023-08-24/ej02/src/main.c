#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

void run_ls(int pipe_fd[]) {
  close(pipe_fd[READ]); // No es necesario.
  dup2(pipe_fd[WRITE], STDOUT_FILENO);
  char* argv[3] = {"/bin/ls", "-al", NULL};
  execvp(argv[0], argv);
}

void run_wc(int pipe_fd[]) {
  close(pipe_fd[WRITE]); // Necesario para que wc reciba EOF.
  dup2(pipe_fd[READ], STDIN_FILENO);
  char* argv[3] = {"/bin/wc", "-l", NULL};
  execvp(argv[0], argv);
}

int main(int argc, char const* argv[]) {
  int fd[2];
  pipe(fd);

  pid_t pid_ls = fork();
  if (pid_ls == 0) run_ls(fd);

  pid_t pid_wc = fork();
  if (pid_wc == 0) run_wc(fd);

  close(fd[0]);
  close(fd[1]);

  // Esperamos que los 2 procesos hijos terminen antes de terminar el padre.
  // De esta forma el output de ls -al | wc -l se imprime correctamente antes de que
  // el shell retome el control y muestre el prompt al usuario.
  wait(NULL);
  wait(NULL);

  return 0;
}
