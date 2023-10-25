from struct import pack, unpack
import sys

p32 = lambda x: pack("<L", x)
u32 = lambda x: unpack("<I", x)[0]
encrypt = lambda data: bytes(map(lambda x: (((x << 4) | (x >> 4))&0xff) ^ 0xff, data))


def main():
    f = open(sys.argv[1], "rb")
    data = bytearray(f.read())
    f.close()

    if data.find(b"HYGFK87C") == -1:
        print("Please Input decrypted firmware")
        exit()
    
    checksum = -1
    
    for i in range(0, len(data)-4, 4):
      checksum += u32(data[i:i+4])

    checksum &= 0xffffffff
    checksum ^= 0xffffffff

    fixed = data[:-4] + p32(checksum)
    
    f = open(sys.argv[1] + "-fixed.bin", "wb")
    f.write(encrypt(fixed))
    f.close()

    print("Done")
    
if __name__ == '__main__':
  main()
