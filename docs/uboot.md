<!-- 
@see https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/verified-boot.txt

@see https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/classes/uboot-sign.bbclass
@see https://wiki.yoctoproject.org/wiki/TipsAndTricks/Patching_the_source_for_a_recipe
@see https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/signature.txt

@see https://github.com/NVISO-BE/VerifiedBootRPi3/blob/master/instructions.md
@see https://lxr.missinglinkelectronics.com/uboot/doc/README.fdt-control
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

Edit [`verified-boot.inc`](https://meta-raspberrypi.readthedocs.io/en/latest/extra-build-config.html#boot-to-u-boot)
```bash
RPI_USE_U_BOOT = "1"
```

## U-Boot verification keys

Following [U-Boot signature](https://github.com/ARM-software/u-boot/blob/master/doc/uImage.FIT/signature.txt) tutorial and [this use case](https://patchwork.openembedded.org/patch/156470/)
```bash
cd rpi-build/conf
mkdir keys && cd keys
openssl genrsa -F4 -out dev.key 2048
openssl req -batch -new -x509 -key dev.key -out dev.crt
```

## U-Boot configuration from Yocto recipe

From indications in [`meta/classes/uboot-sign.bbclass`](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/classes/uboot-sign.bbclass) add this following configuration to `verified-boot.inc`
```bash
# @see https://patchwork.openembedded.org/patch/156470/
UBOOT_SIGN_KEYDIR = "${TOPDIR}/conf/keys"
UBOOT_SIGN_KEYNAME = "dev"
UBOOT_MKIMAGE_DTCOPTS = "-I dts -O dtb -p 2000"
UBOOT_SIGN_ENABLE = "1"

KERNEL_DEVICETREE = "bcm2710-rpi-3-b-plus.dtb"
```

## Emit FIT Image containing signatures

In `verified-boot.inc`
```bash
KERNEL_CLASSES ?= "kernel-fitimage"
KERNEL_IMAGETYPE = "fitImage"
```

# U-Boot source configuration

Following [devtool documentation](https://wiki.yoctoproject.org/wiki/TipsAndTricks/Patching_the_source_for_a_recipe) we can patch the sources of any recipe.  

1. Select the recipe to modify
```bash
devtool modify u-boot
cd rpi-build/workspace/sources/u-boot/
```

2. Edit `include/configs/rpi.h:12` file to enable `verified boot`
```c
#define CONFIG_OF_SEPARATE
```

3. Commit the changes
```bash
git commit -am "CONFIG_OF_SEPARATE in include/configs/rpi.h"
```

4. Generate `.patch` files and `.bbappend` file to modify the recipe
```bash
cd ../../../../
# in sources/yocto
devtool update-recipe -a ../meta-homeautomation u-boot
```

5. Clean the workspace
```bash
devtool reset u-boot
```

6. Now you should be able to build
```bash
bitbake core-image-homeautomation
```

# Troubleshooting

The `meta-raspberrypi` BSP layer write the image. Sadly, this layer only supports the old image format and boot system known as `uImage` (for u-boot images).  
It could be possible to declare our own image handler (`homeautomation:do_image_fit`) just to be able to build / partition the image and configure the bootloader correctly.  
Anyway, we would need to resolve incompatibilities with [`Mender`](mender.md) because it uses it's own images handlers (`mender:do_image_sdimg` and `mender:do_image_mender`) that uses the `uImage` format.  
This would also break the compatibility between `Mender` and `U-Boot`, make unstable the rollback functionnality and block the update or upgrade of `U-Boot` remotely.

# Standalone build

[Documentation](https://github.com/ARM-software/u-boot/tree/master/doc/uImage.FIT)

[Tutorial](https://blog.nviso.be/2019/04/01/enabling-verified-boot-on-raspberry-pi-3/)

However, it's possible to understand, configure and build `U-Boot` on it's own, without `Yocto`.  
`U-Boot` is normally made to be configured and built as a component.  

<!--
1. Fetch the sources
```bash
git clone https://github.com/ARM-software/u-boot.git
```

2. Configuration


3. FIT image
The new U-Boot image format

First, create the `image.its` file with the following content
```bash
/dts-v1/;
/ {
    description = "RPi FIT Image";
    #address-cells = <2>;
    images {
        kernel-1 {
            description = "default kernel";
            data = /incbin/("Image");
            type = "kernel";
            arch = "arm64";
            os = "linux";
            compression = "none";
            load =  <0x00080000>;
            entry = <0x00080000>;
            hash-1 {
                algo = "sha256";
            };
        };
        fdt-1 {
            description = "device tree";
            data = /incbin/("bcm2710-rpi-3-b.dtb");
            type = "flat_dt";
            arch = "arm64";
            compression = "none";
            hash-1 {
                algo = "sha256";
            };
        };
    };
    configurations {
        default = "config-1";
        config-1 {
            description = "default configuration";
            kernel = "kernel-1";
            fdt = "fdt-1";
            signature-1 {
                algo = "sha256,rsa2048";
                key-name-hint = "dev";
                sign-images = "fdt", "kernel";
            };
        };
    };
};
```

4. Build

Edit [`configs/rpi_3_b_plus_defconfig`](https://github.com/NVISO-BE/VerifiedBootRPi3/blob/master/instructions.md)

```bash
CONFIG_OF_CONTROL=y         # https://github.com/u-boot/u-boot/blob/master/README#L721
CONFIG_OF_SEPARATE=y        # https://github.com/u-boot/u-boot/blob/master/README#L738
CONFIG_FIT=y                # https://github.com/u-boot/u-boot/blob/master/Kconfig
CONFIG_FIT_SIGNATURE=y      # https://github.com/u-boot/u-boot/blob/master/Kconfig
CONFIG_RSA=y

FIT_ENABLE_SHA256_SUPPORT=y

FIT_VERBOSE=y               # debug
```

`make rpi_3_b_plus_defconfig` will take care of defining our C headers from the defconfig file


## FIT Images

## Verified boot

chip family BCM2710 
chip implementation BCM2837
https://github.com/torvalds/linux/blob/master/arch/arm/boot/dts/bcm2837-rpi-3-b-plus.dts
-->