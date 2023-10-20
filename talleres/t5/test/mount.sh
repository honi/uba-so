#!/bin/bash

rm -rf hdd
mkdir hdd
losetup -o 2097152 /dev/loop5 hdd.raw
mount /dev/loop5 hdd
