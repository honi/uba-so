#include <stdlib.h>  // srand() rand()
#include <time.h>    // time()
#include <unistd.h>  // getpid()

int tirar_dado() {
  srand(time(NULL) ^ (getpid() << 16));
  return (rand() % 6) + 1;
}