## Ejercicio 2

**Me dieron la mitad de los puntos porque aparentemente las tareas "real-time" si o si quieren que las pongamos con prioridad alta... Pero me da la sensación que no leyeron mi justificación, para mi "real-time" era solo un nombre, porque la descripción de las tareas y más aún tratándose de un jueguito no corresponde a tareas real-time mission-critical.**

Podemos utilizar una cola multi nivel donde los procesos tienen prioridades estáticas, no se mueven entre colas.

- Nivel 0 (prioridad alta): FCFS para procesos interactivos.
- Nivel 1 (prioridad baja): Round-robin para procesos real-time.

Priorizamos a los procesos interactivos para minimizar la latencia del juego que percibe el jugador. Pero más importante aún, porque asumimos que estos procesos tienen ráfagas muy cortas de CPU, pues solo revisan el teclado o mouse y luego actualizan las estructuras de datos internas del juego. Incluso podrían crear nuevos procesos real-time para las nuevas unidades que construye el jugador. Utilizamos una cola FCFS para responder a las interacciones del usuario en el orden que sucedieron, ya que eso es lo naturalmente esperado en un juego.

Los procesos real-time que controlan a las unidades del juego se procesan con una cola round-robin para intentar garantizar un uso justo del CPU. Podemos llegar a tener miles de unidades en juego, y cada una tiene que actualizar su estado interno constantemente (posición, vida, etc). Para optimizar el uso del CPU, los procesos real-time pueden ceder su quantum antes de tiempo si no están haciendo nada, o están muy lejos del combate fuera de la pantalla, o son unidades inmovilizadas (por ejemplo por algún poder u arma del enemigo).

También podríamos considerar utilizar una cola EDF (earliest deadline first). Cada proceso define su deadline en base a la acción que está realizando. Si la unidad se está moviendo o atacando debemos asegurarnos de obtener el CPU pronto, caso contrario podemos esperar más tiempo antes de obtener el CPU. Pero como pueden haber miles de unidades en juego, ordenar constantemente los procesos por su deadline podría resultar en peor performance, por eso optamos por round-robin.
