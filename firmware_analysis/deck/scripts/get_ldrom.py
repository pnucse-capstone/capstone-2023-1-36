## Linux Only

import usb.core
import usb.util

def setup_device(VID=0x0f39, PID=0x0211):
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

    try:
        usbdev.set_configuration()
    except:
        pass

    ctrl_req = {
            'bmRequestType': 0x81,
            'bRequest': 0x06,
            'wValue': 0x2100,
            'wIndex': 0x01,
            'data_or_wLength': 4096
    }

    data = usbdev.ctrl_transfer(**ctrl_req,  timeout=5000)
    data = bytearray(data)

    print(len(data))
    print()

    for i in range(0, len(data)):
        print("%02x "%(data[i]), end='')
    print()

    f = open("ldrom.bin", "wb")
    f.write(data)
    f.close()

if __name__ == "__main__":
    main()
