RPI_USE_U_BOOT = "1"

UBOOT_SIGN_KEYDIR = "${TOPDIR}/conf/keys"
UBOOT_SIGN_KEYNAME = "dev"
UBOOT_MKIMAGE_DTCOPTS = "-I dts -O dtb -p 2000"

KERNEL_CLASSES ?= " kernel-fitimage "
KERNEL_IMAGETYPE = "fitImage"

KERNEL_DEVICETREE = "bcm2710-rpi-3-b-plus.dtb"
#do_image_rpi_sdimg[depends] += "u-boot:do_deploy"
