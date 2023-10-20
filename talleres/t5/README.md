# Taller Ext-2

* Los ejercicios consisten en completar 3 funciones en el archivo `src/ext2fs.cpp`

* Usen la VM de la materia. Para levantarla:

  1. Ejecutar `vagrant up` y esperar
  2. Luego `vagrant ssh` para abrir una terminal dentro de la VM.
  3. Finalmente, `cd /vagrant` para ir al directorio montado con el código del taller.

* Para testear la implementación:

  1. Ir a la carpeta **test** (`cd test`)
  2. Descomprimir el archivo hdd.raw.xz con el comando `unxz hdd.raw.xz`
  3. Compilar con `make`
  4. Ejecutar `./testhdd hdd.raw`

* También pueden montar el disco con `./mount.sh`
