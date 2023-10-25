import binascii
from Crypto.Cipher import DES
from struct import pack, unpack

u16_B = lambda x: unpack(">H", x)[0]

def main():
    with open("GK893B V1.0.25_20210702.bin", "r") as f:
        data = f.read().strip().split("\n")

    block_dict = {}
    decryptor = DES.new(binascii.unhexlify("7200000000000000"), DES.MODE_ECB)
    result = b""

    for line in data:
        _len = int(line[1:3], 16)
        blocks = b"".join([decryptor.decrypt(binascii.unhexlify(line[b:b+16])) for b in range(3, len(line), 16)])

        size, offset, _type, data_block, checksum = blocks[0], u16_B(blocks[1:3]), blocks[3], blocks[4:_len-1], blocks[_len-1]
        calculated_checksum = ((sum(blocks[:_len-1]) ^ 0xff) + 1) & 0xff

        pad = blocks[_len:]

        print(_type)
        
        assert size == len(data_block)
        assert pad == b"\x00" * len(pad)
        assert calculated_checksum == checksum

        if size == 16:
            print(offset, blocks)
            result += data_block
            

    with open("decrypted.bin", "wb") as f:
        f.write(result)

if __name__ == "__main__":
    main()
