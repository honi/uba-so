#!/usr/bin/env python3
import unittest
import os


class Letras123TestCase(unittest.TestCase):
    def test_abrir_tres_veces(self):
        # Abro tres veces el dispositivo
        dev1 = os.open("/dev/letras123", os.O_RDWR)
        dev2 = os.open("/dev/letras123", os.O_RDWR)
        dev3 = os.open("/dev/letras123", os.O_RDWR)

        # Cierro lo que abrí
        os.close(dev1)
        os.close(dev2)
        os.close(dev3)

    def test_abrir_cuatro_veces_falla(self):
        # Abro tres veces el dispositivo
        dev1 = os.open("/dev/letras123", os.O_RDWR)
        dev2 = os.open("/dev/letras123", os.O_RDWR)
        dev3 = os.open("/dev/letras123", os.O_RDWR)

        # La cuarta vez debería fallar
        with self.assertRaises(OSError):
            dev4 = os.open("/dev/letras123", os.O_RDWR)

        # Cierro lo que abrí
        os.close(dev1)
        os.close(dev2)
        os.close(dev3)

    def test_abrir_cuatro_veces_cerrando_previamente_el_tercero(self):
        # Abro tres veces el dispositivo
        dev1 = os.open("/dev/letras123", os.O_RDWR)
        dev2 = os.open("/dev/letras123", os.O_RDWR)
        dev3 = os.open("/dev/letras123", os.O_RDWR)

        # No debería fallar si libero un espacio antes
        os.close(dev3)
        dev4 = os.open("/dev/letras123", os.O_RDWR)

        # Cierro lo que abrí
        os.close(dev1)
        os.close(dev2)
        os.close(dev4)

    def test_leer_sin_haber_escrito_falla(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Tiene que fallar si intento leer sin haber escrito previamente
        with self.assertRaises(OSError):
            os.read(dev, 1)

        # Cierro el dispositivo
        os.close(dev)

    def test_escribir_una_letra(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Escribo un string
        os.write(dev, b'A')

        # Cierro el dispositivo
        os.close(dev)

    def test_leer_despues_de_escribir_una_letra(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Escribo una letra
        os.write(dev, b'A')

        # Leo un caracter de lo que escribí
        data = os.read(dev, 1)

        self.assertEqual(data, b'A')

        # Cierro el dispositivo
        os.close(dev)

    def test_escribir_un_string(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Escribo un string
        os.write(dev, b'Hola Sistemas Operativos')

        # Cierro el dispositivo
        os.close(dev)

    def test_leer_despues_de_escribir_un_string(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Escribo un string
        os.write(dev, b'Hola Sistemas Operativos')

        # Leo un caracter de lo que escribí
        data = os.read(dev, 1)
        self.assertEqual(data, b'H')

        # Leo varios caracteres de lo que escribí
        data2 = os.read(dev, 10)
        self.assertEqual(data2, b'HHHHHHHHHH')

        # Cierro el dispositivo
        os.close(dev)

    def test_escribir_multiples_veces_no_afecta_el_read(self):
        # Abro el dispositivo
        dev = os.open("/dev/letras123", os.O_RDWR)

        # Escribo un string
        os.write(dev, b'Hola Sistemas Operativos')

        # Escribo un segundo string
        os.write(dev, b'Me confundi no queria escribir esto')

        # Leo un caracter de lo que escribí
        data = os.read(dev, 1)
        self.assertEqual(data, b'H')

        # Escribo un tercer string
        os.write(dev, b'Chau Sistemas Operativos')

        # Leo un string de lo que escribí
        data2 = os.read(dev, 10)
        self.assertEqual(data2, b'HHHHHHHHHH')

        # Cierro el dispositivo
        os.close(dev)

    def test_multiples_operaciones(self):
        # Abro tres veces el dispositivo
        dev1 = os.open("/dev/letras123", os.O_RDWR)
        dev2 = os.open("/dev/letras123", os.O_RDWR)
        dev3 = os.open("/dev/letras123", os.O_RDWR)

        # Escribo un string en el primer dispositivo
        os.write(dev1, b'Hola Sistemas Operativos')

        # Escribo un string distinto en el segundo dispositivo
        os.write(dev2, b'Chau Sistemas Operativos')

        # Leo un caracter de lo que escribí en el primer dispositivo
        data = os.read(dev1, 1)
        self.assertEqual(data, b'H')

        # Leo un caracter de lo que escribí en el segundo dispositivo
        data2 = os.read(dev2, 1)
        self.assertEqual(data2, b'C')

        # Leer el tercer dispositivo tiene que fallar ya que no escribí nada
        with self.assertRaises(OSError):
            os.read(dev3, 1)

        # Escribo nuevamente en el segundo dispositivo
        os.write(dev2, b'Saludo de nuevo...')

        # Leo un string de lo que escribí en el segundo dispositivo
        data3 = os.read(dev2, 10)
        self.assertEqual(data3, b'CCCCCCCCCC')

        # Falla si intento abrir un cuarto dispositivo
        with self.assertRaises(OSError):
            dev4 = os.open("/dev/letras123", os.O_RDWR)

        # No debería fallar si libero un espacio antes
        os.close(dev2)
        dev4 = os.open("/dev/letras123", os.O_RDWR)

        # Leer el cuarto dispositivo tiene que fallar ya que no escribí nada
        with self.assertRaises(OSError):
            os.read(dev4, 1)

        # Escribo al tercer dispositivo
        os.write(dev3, b'Me estoy cansando de testear')

        # Leo el tercer dispositivo
        data4 = os.read(dev3, 5)
        self.assertEqual(data4, b'MMMMM')

        # Cierro los dispositivos restantes
        os.close(dev1)
        os.close(dev3)
        os.close(dev4)


if __name__ == '__main__':
    unittest.main()
