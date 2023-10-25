#!/bin/bash

arm-none-eabi-gcc -Os -c -mthumb -mcpu=cortex-m0 -o code.o code.c
ls -al code.o
nm code.o
