import usb.core
import usb.util
import binascii
import time
import tqdm

def setup_device(VID=0x0483, PID=0x5131):
    usbdev = usb.core.find(idVendor=VID, idProduct=PID)

    if usbdev is None:
        print("Device Not Found")
        return False

    for cfg in usbdev:
        for idx in range(cfg.bNumInterfaces):
            if usbdev.is_kernel_driver_active(idx):
                usbdev.detach_kernel_driver(idx)
                print("Detached")

    return usbdev

def main():
    usbdev = setup_device()
    usbdev.reset()

    usbdev.write(1, b"\x00\xf9" + b"\x00" * 62)
    read = b""

    for i in range(16):
        read += usbdev.read(0x82, 64)
        print(i, len(read))

    f = open("read.bin", "wb")
    f.write(read)
    f.close()


if __name__ == "__main__":
    main()
