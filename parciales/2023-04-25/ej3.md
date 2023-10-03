# Ejercicio 3

```c
semaphore muelle = sem(0);
semaphore orilla = sem(0);
semaphore barreraLobos = sem(0);
semaphore barreraCabras = sem(0);
atomic<int> enElBote = 0;
int numLobos = 0;
int numCabras = 0;

void lobo() {
    muelle.wait();
    numLobos++;
    if (numLobos == 4) {
        numLobos -= 4;
        barreraLobos.signal(4);
    } else if (numLobos == 2 && numCabras >= 2) {
        numLobos -= 2;
        numCabras -= 2;
        barreraLobos.signal(2);
        barreraCabras.signal(2);
    } else {
        muelle.signal();
    }

    barreraLobos.wait();
    abordar();

    if (enElBote.getAndInc() == 3) {
        remar();
        orilla.signal(3);
    } else {
        orilla.wait();
    }

    if (enElBote.getAndDec() == 1) {
        muelle.signal();
    }
}

void cabra() {
    muelle.wait();
    numCabras++;
    if (numCabras == 4) {
        numCabras = 0;
        barreraCabras.signal(4);
    } else if (numCabras == 2 && numLobos >= 2) {
        numCabras = 0;
        numLobos -= 2;
        barreraCabras.signal(2);
        barreraLobos.signal(2);
    } else {
        muelle.signal();
    }

    barreraCabras.wait();
    abordar();

    if (enElBote.getAndInc() == 3) {
        remar();
        orilla.signal(3);
    } else {
        orilla.wait();
    }

    if (enElBote.getAndDec() == 1) {
        muelle.signal();
    }
}
```
