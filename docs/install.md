# Configuration

```bash
git clone git@gitlab.com:Nathan675/home-automation-box-security.git homeautomation
cd homeautomation/sources/yocto
source oe-init-build-env rpi3-build

bitbake-layers remove-layer ../meta-poky
bitbake-layers remove-layer ../meta-yocto-bsp
bitbake-layers add-layer ../../meta-raspberrypi
bitbake-layers add-layer ../../meta-homeautomation
```

Edit `rpi3-build/conf/local.conf`
```bash
#MACHINE ??= "qemux86"
MACHINE ?= "raspberrypi3"
#DISTRO ?="poky"
DISTRO ?= "homeautomation"
```

# Build

```bash
bitbake core-image-homeautomation
```

