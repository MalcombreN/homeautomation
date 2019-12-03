<!-- 
@see https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/verified-boot.txt

@see https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/classes/uboot-sign.bbclass
@see https://wiki.yoctoproject.org/wiki/TipsAndTricks/Patching_the_source_for_a_recipe
@see https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/signature.txt
-->

# Git setup

from `homeautomation/yocto` branch
```bash
# update current branch before branching
git pull
# create branch
git branch uboot
git checkout uboot
# push branch to remote
git push --set-upstream ... uboot
```

# Yocto setup

Create `homeautomation/sources/meta-homeautomation/conf/distro/include/verified-boot.inc`

Include it at the end of the distribution configuration `homeautomation/sources/meta-homeautomation/conf/distro/homeautomation.conf`
```bash
require include/verified-boot.inc
```

## Enable U-Boot support from the meta-raspberrypi layer
Edit `verified-boot.inc`[1]
```bash
RPI_USE_U_BOOT = "1"
```
[1](https://meta-raspberrypi.readthedocs.io/en/latest/extra-build-config.html#boot-to-u-boot)

## U-Boot verification keys
Following [U-Boot signature](https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/signature.txt) tutorial and [this use case](https://patchwork.openembedded.org/patch/156470/)
```bash
cd rpi-build/conf
mkdir keys && cd keys
openssl genrsa -F4 -out dev.key 2048
openssl req -batch -new -x509 -key dev.key -out dev.crt

#openssl genpkey -algorithm RSA -out dev.key -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:6553
#openssl req -batch -new -x509 -key dev.key -out dev.crt
```

## U-Boot configuration from Yocto recipe 
From indications in [`meta/classes/uboot-sign.bbclass`](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/classes/uboot-sign.bbclass) edit `verified-boot.inc`
```bash
# @see https://patchwork.openembedded.org/patch/156470/
UBOOT_SIGN_KEYDIR = "${TOPDIR}/conf"
UBOOT_SIGN_KEYNAME = "dev"
UBOOT_MKIMAGE_DTCOPTS = "-I dts -O dtb -p 2000"
UBOOT_SIGN_ENABLE = "1"
```

# Emit FIT Image containing signed kernel
Edit `verified-boot.inc`
```bash
KERNEL_CLASSES ?= "kernel-fitimage"
KERNEL_IMAGETYPE ?= "fitImage"
```

facon degueu
```bash
bitbake core-image-homeautomation
vi tmp/work/machine/u-boot/release/git/include/configs/rpi.h
#define CONFIG_OF_SEPARATE
bitbake -C compile u-boot ?
bitbake core-image-homeautomation
```

# U-Boot source configuration
Following [devtool documentation](https://wiki.yoctoproject.org/wiki/TipsAndTricks/Patching_the_source_for_a_recipe) we can patch the sources of any recipe

Select the recipe to modify
```bash
devtool modify u-boot
cd rpi-build/workspace/sources/u-boot/
```

Edit `include/configs/rpi.h:12` file to enable `verified boot`
```c
#define CONFIG_OF_SEPARATE
```

Commit the changes
```bash
git commit -am "CONFIG_OF_SEPARATE in include/configs/rpi.h"
```

Generate `.patch` files and `.bbappend` file to modify the recipe
```bash
cd ../../../../
# in homeautomation/sources/yocto
devtool update-recipe -a ../meta-homeautomation u-boot
```

Cleaning
```bash
devtool reset u-boot
```

Now you should be able to build
```bash
bitbake core-image-homeautomation
```