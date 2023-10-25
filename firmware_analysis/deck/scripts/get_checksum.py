from struct import pack, unpack
u32 = lambda x: unpack("<I", x)[0]

def main():
    f = open("L2226V301.bin-dec", "rb")
    data = f.read()
    f.close()

    result = -1
    
    for i in range(0, len(data)-4, 4):
      result += u32(data[i:i+4])

    result &= 0xffffffff
    result ^= 0xffffffff
    
    print(hex(result))
if __name__ == '__main__':
  main()
