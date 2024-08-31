#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() write() read()
#include "dado.h"    // tirar_dado()

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };
// Constants 0 and 1 for LESTER and ELIZA
enum { LESTER, ELIZA };

// Variables globales
int pipes[2][2];

void lester() {
  // Tiro el dado
  int dado_lester = tirar_dado();

  // Le informo el resultado a Humberto
  write(pipes[LESTER][WRITE], &dado_lester, sizeof(dado_lester));
  exit(EXIT_SUCCESS);
}

void eliza() {
  // Tiro el dado
  int dado_eliza = tirar_dado();

  // Le informo el resultado a Humberto
  write(pipes[ELIZA][WRITE], &dado_eliza, sizeof(dado_eliza));
  exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
  // Creo los pipes
  for(int i=0; i<2; i++){
    pipe(pipes[i]);
  }

  // Creo a Lester
  pid_t pid_lester = fork();
  if(pid_lester==0){
    lester();
  }

  // Creo a Eliza
  pid_t pid_eliza = fork();
  if(pid_eliza==0){
    eliza();
  }

  // Recibo el dado de Lester
  int dado_lester;
  read(pipes[LESTER][READ], &dado_lester, sizeof(dado_lester));
  printf("Soy Lester y mi dado es: %d\n", dado_lester);

  // Recibo el dado de Eliza
  int dado_eliza;
  read(pipes[ELIZA][READ], &dado_eliza, sizeof(dado_eliza));
  printf("Soy Eliza y mi dado es: %d\n", dado_eliza);

  // Anuncio al ganador
  if (dado_lester == dado_eliza) {
    printf("Empate\n");
  } else if (dado_lester > dado_eliza) {
    printf("Ganó Lester\n");
  } else {
    printf("Ganó Eliza\n");
  }

  return 0;
}
