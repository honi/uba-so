#include "structs.h"

#define INT_LLEGUE 0
#define REG_X 0
#define REG_Y 1
#define REG_START 2

semaphore llegue;
semaphore mutex;

void llegue_handler() {
    llegue.signal();
}

void driver_init() {
    sem_init(&llegue, 0);
    sem_init(&mutex, 1);
    request_irq(INT_LLEGUE, llegue_handler);
}

void driver_remove() {
    free_irq(INT_LLEGUE);
}

int driver_write(char* udata, int size) {
    assert(size == sizeof(posicion));

    posicion pos;
    copy_from_user(&pos, udata, size);

    mutex.wait();
    OUT(REG_X, pos.x);
    OUT(REG_Y, pos.y);
    OUT(REG_START, 1);
    llegue.wait();
    mutex.signal();

    return size;
}
