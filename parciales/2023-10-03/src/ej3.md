## Ejercicio 3

### LRU

Least recently used mantiene una cola de desalojo ordenada por el último tiempo de acceso a la página. Cuando necesitamos desalojar una página, elegimos la que se usó hace más tiempo, con la suposición de que si no se accede hace mucho tiempo entonces no es una página muy usada por los procesos ready.

| Página solicitada | F1 | F2 | F3 | F4 | Pafe fault | Cola de desalojo |
|-------------------|----|----|----|----|------------|------------------|
| Estado inicial    | 0  | 1  | 2  | 3  | -          | 0 1 2 3          |
| 2                 | 0  | 1  | 2  | 3  | no         | 0 1 3 2          |
| 1                 | 0  | 1  | 2  | 3  | no         | 0 3 2 1          |
| 7                 | 7  | 1  | 2  | 3  | si         | 3 2 1 7          |
| 3                 | 7  | 1  | 2  | 3  | no         | 2 1 7 3          |
| 0                 | 7  | 1  | 0  | 3  | si         | 1 7 3 0          |
| 5                 | 7  | 5  | 0  | 3  | si         | 7 3 0 5          |
| 2                 | 2  | 5  | 0  | 3  | si         | 3 0 5 2          |
| 1                 | 2  | 5  | 0  | 1  | si         | 0 5 2 1          |
| 2                 | 2  | 5  | 0  | 1  | no         | 0 5 1 2          |
| 9                 | 2  | 5  | 9  | 1  | si         | 5 1 2 9          |
| 5                 | 2  | 5  | 9  | 1  | no         | 1 2 9 5          |
| 7                 | 2  | 5  | 9  | 7  | si         | 2 9 5 7          |
| 3                 | 3  | 5  | 9  | 7  | si         | 9 5 7 3          |
| 8                 | 3  | 5  | 8  | 7  | si         | 5 7 3 8          |
| 5                 | 3  | 5  | 8  | 7  | no         | 7 3 8 5          |

Tasa promedio de page faults: `11 pages faults / 15 accesos = 0.73`

### Second chance

Esta política utiliza una cola de desalojo FIFO con el agregado del second chance. Al cargar una página en memoria, se inicializa un bit de accedido en 0. Cada vez que se accede a una página se marca su bit accedido en 1 (en la tabla lo marcamos con un *). Cuando tenemos que desalojar una página, tomamos el tope de la cola FIFO, pero si está marcada, entonces le sacamos el bit de accedido y la reinsertamos al final de la cola. En efecto, le dimos a la página una segunda oportunidad. Repetimos este proceso hasta encontrar una página que no esté marcada.

| Página solicitada | F1 | F2 | F3 | F4 | Pafe fault | Cola de desalojo |
|-------------------|----|----|----|----|------------|------------------|
| Estado inicial    | 0  | 1  | 2  | 3  | -          | 0 1 2 3          |
| 2                 | 0  | 1  | 2  | 3  | no         | 0 1 2* 3         |
| 1                 | 0  | 1  | 2  | 3  | no         | 0 1* 2* 3        |
| 7                 | 7  | 1  | 2  | 3  | si         | 1* 2* 3 7        |
| 3                 | 7  | 1  | 2  | 3  | no         | 1* 2* 3* 7       |
| 0                 | 0  | 1  | 2  | 3  | si         | 2* 3* 7 1 `->` 3* 7 1 2 `->` 7 1 2 3 `->` 1 2 3 0 |
| 5                 | 0  | 5  | 2  | 3  | si         | 2 3 0 5          |
| 2                 | 0  | 5  | 2  | 3  | no         | 2 3* 0 5         |
| 1                 | 0  | 5  | 1  | 3  | si         | 3* 0 5 1         |
| 2                 | 2  | 5  | 1  | 3  | si         | 0 5 1 3 `->` 5 1 3 2 |
| 9                 | 2  | 9  | 1  | 3  | si         | 1 3 2 9          |
| 5                 | 2  | 9  | 5  | 3  | si         | 3 2 9 5          |
| 7                 | 2  | 9  | 5  | 7  | si         | 2 9 5 7          |
| 3                 | 3  | 9  | 5  | 7  | si         | 9 5 7 3          |
| 8                 | 3  | 8  | 5  | 7  | si         | 5 7 3 8          |
| 5                 | 3  | 8  | 5  | 7  | no         | 5* 7 3 8         |

Tasa promedio de page faults: `10 pages faults / 15 accesos = 0.66`
