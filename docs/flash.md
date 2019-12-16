# Flash an image into Raspberry Pi 3B+

Replace sdX with your sdcard device (sdb / mmcblk0 ...), you can find the device name with the `dmesg` command.
```bash
# Unmount all the partitions
$ sudo umount /dev/sdX1
# Repeat the operation as many as there are partitions
$ sudo umount /dev/sdX2
# Flash using dd
$ sudo dd if=sources/yocto/rpi-build/tmp-glibc/deploy/images/raspberrypi3/core-image-homeautomation-raspberrypi3.sdimg of=/dev/sdX bs=1M status=progress
# Flush the disk caches
$ sudo sync
```

# Connect to the box using UART

Plug an UART to USB adapter from your machine to the raspberry pi 3b+ using this configuration :
```bash
Red     5V          pin 4 (do not plug it if you use external power source)
Black   GND         pin 6
White   USB RX      pin 8
Green   USB TX      pin 10
```

Then connect to the raspi from machine
```bash
# to find which device is the USB to UART controller
$ sudo dmesg
# to connect to the card
$ sudo minicom -D /dev/ttyUSB0 -b 115200
```
<!--
USB to UART

2 5V
4 5V
6 GND
8 UART TX
10 UART RX

Red     5V
Black   GND
White   USB RX
Green   USB TX

Bleu    GND
Vert    RX
Rouge   TX
-->