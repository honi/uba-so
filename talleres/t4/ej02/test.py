#!/usr/bin/env python3
import unittest
import os


class AzarTestCase(unittest.TestCase):
    def test_basico(self):
        dev = os.open("/dev/azar", os.O_RDWR)

        os.write(dev, bytes(str(100), 'ascii'))

        rawdata = os.read(dev, 32)
        number = int(rawdata)
        self.assertGreaterEqual(number, 0)
        self.assertLessEqual(number, 100)

        os.close(dev)

if __name__ == '__main__':
    unittest.main()
