typedef struct {
    int x;
    int y;
    int codigo;
} paquete;

typedef struct {
    int x;
    int y;
} posicion;

// En vez de usar constantes para cada acción podríamos haber usado un struct para
// solicitar varias acciones en simultáneo.
const int BRAZO_SUBIR = 0; // Sube 1 estante.
const int BRAZO_BAJAR = 1; // Baja 1 estante.
const int BRAZO_EXTENDER = 2;
const int BRAZO_CONTRAER = 3;
const int BRAZO_AGARRAR = 4;
const int BRAZO_SOLTAR = 5;
