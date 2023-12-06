## Ejercicio 1

**Está mal, no cumple con los requerimientos.**

La solución a este ejercicio está en "The Little Book of Semaphores", el problema llamado "Sushi Bar".

```c
#define N 10
semaphore molinete = sem(1);
semaphore barrera = sem(N);
atomic<int> esperando = 0;
atomic<int> comprando = 0;
bool congestionado = false;
bool liberarMolinete = false;

void usuario() {
    // El molinete controla los usuarios que quieren entrar al sistema para comprar.
    molinete.wait();
    molinete.signal();

    // La barrera solo permite que N=10 usuarios compren concurrentemente.
    // Con la variable atómica esperando contabilizamos cuántos usuarios esperan para comprar.
    esperando.getAndInc();
    barrera.wait();
    esperando.getAndDec();

    // Si somos el usuario número N=10 que está comprando entonces el sistema está congestionado.
    if (comprando.getAndInc() == N-1) {
        congestionado = true;

        // Si el sistema estaba congestionado en la tanda anterior de N=10 usuarios,
        // cuando terminan de entrar los nuevos N=10 usuarios rehabilitamos el molinete
        // asi los nuevos usuarios pueden avanzar a la "sala de espera" (por la barrera).
        // De esta forma ningún usuario puede pasar el molinete antes de que pasen los
        // próximos N=10 usuarios que ya estaban esperando desde antes por la barrera.
        if (liberarMolinete) {
            molinete.signal();
            liberarMolinete = false;
        }
    }

    comprar_ticket();

    if (congestionado) {
        // Como el sistema está congestionado, necesitamos esperar que terminen de
        // comprar los N=10 usuarios antes de habilitar el acceso al resto.
        if (comprando.getAndDec() == 1) {
            congestionado = false;

            // Si hay más de N=10 usuarios esperando, entonces activamos el mecanismo
            // para evitar que otros usuarios se colen.
            if (esperando.get() >= N) {
                // Pedimos acá el molinete y no lo liberamos aún, en efecto bloqueando a
                // todos los usuarios nuevos que llegan al sistema a partir de ahora.
                molinete.wait();
                liberarMolinete = true;
            }

            // Ahora sí podemos habilitar que pase la próxima tanda de N=10 usuarios.
            barrera.signal(N);
        }
    } else {
        // Si el sistema no está congestionado entonces habilitamos un usuario nuevo
        // para que pase la barrera y compre su ticket.
        comprando.getAndDec();
        barrera.signal();
    }
}
```
