# Práctica 1 / Ejercicio 8

Al realizar el fork, el nuevo proceso tiene su propio espacio de memoria con la variable `dato` inicializada en 0. Al modificar esta variable, se modifica dentro del espacio de memoria de cada proceso. Las modificaciones a la variable `dato` realizadas por el proceso hijo son solo visibles para ese proceso, por eso imprime `1`, `2` y `3`. Como el proceso padre nunca modifica la variable, siempre imprime el mismo valor de inicialización `0`.
