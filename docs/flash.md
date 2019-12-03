

```bash
sudo dd if=yocto/rpi-build/tmp-glibc/deploy/images/raspberrypi3/core-image-homeautomation-raspberrypi3.rpi-sdimg of=/dev/sdb bs=4M status=progress

sudo minicom /dev/ttyUSB0 -b 115200
```

USB to UART

2 5V
4 5V
6 GND
8 UART TX
10 UART RX

Vert    GND     RX      TX      GND     TX      RX
Rouge   TX      GND     RX      RX      GND     TX
Bleu    RX      TX      GND     TX      RX      GND