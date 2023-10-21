#include "structs.h"

// Esta función ya está implementada por el enunciado.
// Retorna el siguiente paquete que debemos buscar.
paquete siguiente_paquete() {}

int main() {
    int ruedas = open("/dev/ruedas");
    int brazo = open("/dev/brazo");
    int lector = open("/dev/lector");

    while (1) {
        // Bloquea hasta que haya algún paquete para buscar.
        paquete p = siguiente_paquete();

        // Bloquea mientras se mueve.
        posicion pos = {.x = p.x, .y = p.y};
        write(ruedas, &pos, sizeof(pos));

        // Revisamos cada estante.
        int estante = 1;
        while (estante <= 10) {
            write(brazo, &BRAZO_EXTENDER, sizeof(int));
            write(brazo, &BRAZO_AGARRAR, sizeof(int));
            write(brazo, &BRAZO_CONTRAER, sizeof(int));

            int codigo;
            read(lector, &codigo, sizeof(codigo));
            if (codigo == p.codigo) {
                // Encontramos el paquete, bajamos el brazo a la altura mínima.
                while (estante-- > 0) write(brazo, &BRAZO_BAJAR, sizeof(int));
                break;
            } else {
                // Probamos con el siguiente.
                write(brazo, &BRAZO_EXTENDER, sizeof(int));
                write(brazo, &BRAZO_SOLTAR, sizeof(int));
                write(brazo, &BRAZO_CONTRAER, sizeof(int));
                write(brazo, &BRAZO_SUBIR, sizeof(int));
                estante++;
            }
        }

        // Llevamos el paquete a la cinta.
        pos.x = 0;
        pos.y = 0;
        write(ruedas, &pos, sizeof(pos));

        // Soltamos el paquete en la cinta.
        write(brazo, &BRAZO_EXTENDER, sizeof(int));
        write(brazo, &BRAZO_SOLTAR, sizeof(int));
        write(brazo, &BRAZO_CONTRAER, sizeof(int));
    }
}
