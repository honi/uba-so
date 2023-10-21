#include "structs.h"

// Al escribir cualquier dato en estos registros el brazo realiza dicha acción instantáneamente.
#define REG_SUBIR 0
#define REG_BAJAR 1
#define REG_EXTENDER 2
#define REG_CONTRAER 3
#define REG_AGARRAR 4
#define REG_SOLTAR 5

int driver_write(char* udata, int size) {
    assert(size == sizeof(int));
    int accion;
    copy_from_user(&accion, udata, size);

    switch (accion) {
        case BRAZO_SUBIR:
            OUT(REG_SUBIR, 1);
            break;
        case BRAZO_BAJAR:
            OUT(REG_BAJAR, 1);
            break;
        case BRAZO_EXTENDER:
            OUT(REG_EXTENDER, 1);
            break;
        case BRAZO_CONTRAER:
            OUT(REG_CONTRAER, 1);
            break;
        case BRAZO_AGARRAR:
            OUT(REG_AGARRAR, 1);
            break;
        case BRAZO_SOLTAR:
            OUT(REG_SOLTAR, 1);
            break;
    }

    return size;
}
