@see https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/classes/uboot-sign.bbclass

## Enable U-Boot support from the meta-raspberrypi layer
```bash
RPI_USE_U_BOOT = "1"
```

## U-Boot configuration from Yocto recipe (meta/classes/uboot-sign.bbclass)
```bash
UBOOT_SIGN_KEYDIR = "${THISDIR}/${PN}"
UBOOT_SIGN_KEYNAME = "dev"
UBOOT_MKIMAGE_DTCOPTS = "-I dts -O dtb -p 2000"
UBOOT_SIGN_ENABLE = "1"
```

## Emit FIT Image containing signed kernel
```bash
KERNEL_CLASSES ?= " kernel-fitimage "
KERNEL_IMAGETYPE ?= "fitImage"


bitbake core-image-homeautomation
vi tmp/work/machine/u-boot/release/git/include/configs/rpi.h
#define CONFIG_OF_SEPARATE
bitbake -C compile u-boot ?
bitbake core-image-homeautomation

@see https://wiki.yoctoproject.org/wiki/TipsAndTricks/Patching_the_source_for_a_recipe
devtool modify u-boot


