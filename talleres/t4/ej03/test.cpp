#include <fstream>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdint>

sem_t rv1[3], rv2[3], rv3, rv4;

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

void *thread_letras(void *_i) {
    intptr_t i = (intptr_t) _i;
    std::fstream dev;
    char buffer[16] = {0};

    switch (i) {
        case 0:
            std::cout << "[1] Abriendo el dispositivo" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(dev.is_open());
            sem_post(&rv1[0]);

            std::cout << "[1] Escribiendo \"AbCdE\"" << std::endl;
            dev << "AbCdE";
            std::cout << "[1] Leyendo 15 caracteres" << std::endl;
            dev.read(buffer, 15);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(strcmp(buffer, "AAAAAAAAAAAAAAA") == 0);

            sem_wait(&rv2[0]);
            std::cout << "[1] Cerrando el dispositivo" << std::endl;
            dev.close();
            assert(!dev.is_open());
            sem_post(&rv3);
            break;
        case 1:
            std::cout << "[2] Abriendo el dispositivo" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(dev.is_open());
            sem_post(&rv1[1]);

            std::cout << "[2] Escribiendo \"Sistemas\"" << std::endl;
            dev << "Sistemas";
            std::cout << "[2] Leyendo 10 caracteres" << std::endl;
            dev.read(buffer, 10);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(strcmp(buffer, "SSSSSSSSSS") == 0);

            std::cout << "[2] Leyendo 0 caracteres" << std::endl;
            dev.read(buffer, 0);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(buffer[0] == 'S');

            sem_wait(&rv2[1]);
            std::cout << "[2] Cerrando el dispositivo" << std::endl;
            dev.close();
            sem_post(&rv4);
            break;
        case 2:
            std::cout << "[3] Abriendo el dispositivo" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(dev.is_open());
            sem_post(&rv1[2]);

            std::cout << "[3] Escribiendo \"hola mundo\"" << std::endl;
            dev << "hola mundo";
            std::cout << "[3] Leyendo 15 caracteres" << std::endl;
            dev.read(buffer, 15);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(strcmp(buffer, "hhhhhhhhhhhhhhh") == 0);

            std::cout << "[3] Escribiendo \"testing\"" << std::endl;
            dev << "testing";
            std::cout << "[3] Leyendo 1 caracter" << std::endl;
            dev.read(buffer, 1);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(strcmp(buffer, "hhhhhhhhhhhhhhh") == 0);

            sem_wait(&rv2[2]);
            std::cout << "[3] Cerrando el dispositivo" << std::endl;
            dev.close();
            break;
        case 3:
            sem_wait(&rv1[0]);
            sem_wait(&rv1[1]);
            sem_wait(&rv1[2]);
            std::cout << "[4] Abriendo el dispositivo (debería fallar)" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(!dev.is_open());
            sem_post(&rv2[0]);
            sem_post(&rv2[1]);
            sem_post(&rv2[2]);

            sem_wait(&rv3);
            std::cout << "[4] Abriendo el dispositivo" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(dev.is_open());

            std::cout << "[4] Intentando leer (debería fallar)" << std::endl;
            assert(!dev.read(buffer, 1));

            std::cout << "[4] Cerrando el dispositivo" << std::endl;
            dev.close();
            break;
        case 4:
            sem_wait(&rv4);
            std::cout << "[5] Abriendo el dispositivo" << std::endl;
            dev.open("/dev/letras123", std::fstream::in | std::fstream::out);
            assert(dev.is_open());

            std::cout << "[5] Escribiendo \"Otra prueba\"" << std::endl;
            dev << "Otra prueba";
            std::cout << "[5] Leyendo 2 caracteres" << std::endl;
            dev.read(buffer, 2);
            std::cout << "* El buffer es: " << buffer << std::endl;
            assert(buffer[1] == 'O');

            std::cout << "[5] Cerrando el dispositivo" << std::endl;
            dev.close();
            break;
    }

    pthread_exit(0);
}

void test_letras123() {
    pthread_t threads[5];
    std::cout << "== Verificando /dev/letras123 ==" << std::endl;

    for (unsigned int i = 0; i < 3; i++) {
        sem_init(&rv1[i], 0, 0);
        sem_init(&rv2[i], 0, 0);
    }
    sem_init(&rv3, 0, 0);
    sem_init(&rv4, 0, 0);

    for (intptr_t i = 0; i < 5; i++) {
        if (pthread_create(&threads[i], NULL, thread_letras, (void *)i)) {
            std::cerr << "Error al crear threads" << std::endl;
            return;
        }
    }

    for (unsigned int i = 0; i < 5; i++) {
        if (pthread_join(threads[i], NULL)) {
            std::cerr << "Error al unir threads" << std::endl;
            return;
        }
    }
}

int main(int argc, char ** argv) {
    test_letras123();
	return 0;
}
