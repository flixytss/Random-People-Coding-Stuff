"""
Just a program to flash the geckoos iso into a usb (Only for linux systems)
"""

import os

if __name__ == "__main__":
    print(
        "select the device you want to flash the iso (do lsblk first unless you want to delete you own drive)"
    )
    dev = input("device: /dev/")
    if not os.path.exists(f"/dev/{dev}"):
        print("that device dosen't exists!")
        exit(1)
    if not os.path.exists("geckoos.iso"):
        print("the iso file dosen't exists! use 'make' to compile it")
        exit(1)
    os.system(f"sudo dd if=geckoos.iso of=/dev/{dev} bs=8M status=progress")
