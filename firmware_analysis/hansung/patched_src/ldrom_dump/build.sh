#!/bin/bash
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m0 -o code.o code.S

arm-none-eabi-objcopy -O binary -j .text code.o /dev/stdout | python3 -c "
import sys;
import binascii;
s=sys.stdin.buffer.read();
print(binascii.hexlify(s).decode())
"
