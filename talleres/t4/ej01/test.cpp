#include <fstream>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdint>

/**
* Tests para los ejercicios del taller de drivers
* ===============================================
*
* - Para que estos tests funcionen, es necesario que los módulos
*   estén cargados en el kernel y sus nodos correspondientes estén creados en
*   el filesystem.
*
* - Tener en cuenta que, por supuesto, que estos tests pasen correctamente
*   no ofrece ninguna garantía de correctitud sobre el código de los drivers.
*   Brindamos estos tests únicamente a modo orientativo.
*/

void test_nulo() {
    std::cout << "== Verificando /dev/nulo ==" << std::endl;
    char buffer[10];

    std::fstream dev;
    std::cout << "* Abriendo dispositivo" << std::endl;
    dev.open("/dev/nulo", std::fstream::in);
    assert(dev.is_open());

    std::cout << "* Intentando lectura" << std::endl;
    dev.read(buffer, 10);
    assert(dev.gcount() == 0);

    std::cout << "* Cerrando dispositivo" << std::endl;
    dev.close();
}

int main(int argc, char ** argv) {
    test_nulo();
	return 0;
}
