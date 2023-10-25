import binascii
from Crypto.Cipher import DES
from struct import pack, unpack

p16_B = lambda x: pack(">H", x)

def main():
    with open("decrypted.bin", "rb") as f:
        data = f.read()

    block_dict = {}
    encryptor = DES.new(binascii.unhexlify("7200000000000000"), DES.MODE_ECB)
    result = []

    for i in range(0, len(data), 16):
        data_block = data[i:i+16]

        if len(data_block) < 16:
            data_block += b"\x00" * (16 - len(data_block))

        size = len(data_block)
        offset = p16_B(len(result)*16)
        _type = bytes([0])
        
        blocks = bytes([size]) + offset + _type + data_block
        checksum = ((sum(blocks) ^ 0xff) + 1) & 0xff
        
        blocks += bytes([checksum])

        padded_blocks = blocks + b"\x00" * ((8 - len(blocks) % 8) % 8)

        
        encrypted_blocks = encryptor.encrypt(padded_blocks)
        result.append(":" + format(0x15, "02X") + binascii.hexlify(encrypted_blocks).decode("utf-8").upper())


    
    
    with open("my-firmware.bin", "w") as f:
        f.write(":0793D4476880C97D8C\n")
        f.write("\n".join(result))
        f.write("\n")
        f.write(":0DE04DBBE1821D7B4A2C8DB1E9BC65EA48\n")
        f.write(":0931FBC0E3C02C40CEAE8E2026E9D5239F\n")
        f.write(":0574DE277D25CC29A9\n")
        

if __name__ == "__main__":
    main()
