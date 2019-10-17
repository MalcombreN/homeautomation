# 
Once you clone the git repository you have to go to

/sources/yocto/

and in commande line write
```bash
source oe-init-build-env rpi3-build
```

now you have to modify bblayers.conf and local.conf

to modify bblayers.conf :
```bash
bitbake-layers remove-layer ../meta-poky
bitbake-layers remove-layer ../meta-yocto-bsp
bitbake-layers add-layer ../../meta-raspberrypi
bitbake-layers add-layer ../../meta-homeautomation
```

now to modify local.conf :
comment the MACHINE variable and add the new one 
```bash
#MACHINE ??= "qemux86"
MACHINE ?= "raspberrypi3"
```
comment the DISTRO variable and add the new one
```bash
#DISTRO ?="poky"
DISTRO ?= "homeautomation"
```
we can now create the image with :
```bash
bitbake core-image-homeautomation
```

