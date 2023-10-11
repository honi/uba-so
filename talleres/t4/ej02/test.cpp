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

void test_azar() {
    std::cout << "== Verificando /dev/azar ==" << std::endl;
    int rand;

    std::fstream dev;
    std::cout << "* Abriendo dispositivo" << std::endl;
    dev.open("/dev/azar", std::fstream::in | std::fstream::out);
    assert(dev.is_open());

    std::cout << "* Configurando limite en 256" << std::endl;
    dev << "256" << std::endl;
    std::cout << "* Output: ";
    for (unsigned int i = 0; i < 16; i++) {
        dev >> rand;
        assert(0 <= rand && rand < 256);
        std::cout << rand << " ";
    }
    std::cout << std::endl;
    std::cout << "* Cerrando dispositivo" << std::endl;
    dev.close();
}

int main(int argc, char ** argv) {
    test_azar();
	return 0;
}
