import sys


def main():
  
  data = open(sys.argv[1], "rb").read()
  result = bytes(map(lambda x:  (((x << 4) | (x >> 4))&0xff) ^ 0xff, data))
  open(sys.argv[1] + "-dec", "wb").write(result)

if __name__ == "__main__":
  main()
