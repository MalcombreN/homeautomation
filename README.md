# Homeautomation

First year cybersecurity degree projet.  
Main goals are being able to perform OTA updates and remote maintenance on an embedded yet secure device.

Technologies used :
* [Yocto](https://yoctoproject.com) to personnalize a [Linux](https://www.linux.org) distribution
* [Mender](https://mender.io) to perform OTA updates
* [SSH](https://en.wikipedia.org/wiki/Secure_Shell) to connect remotely
* [U-Boot](https://www.denx.de/wiki/U-Boot) for a security enforced boot

# Step by step documentation

1. [Setting up Yocto](docs/yocto.md)
2. [Verified Boot using U-Boot](docs/uboot.md)
3. [OTA updates with Mender](docs/mender.md)
4. [Remote maintenance](docs/ssh.md)
5. [Network configuration](docs/network.md)

# Quick start

1. Clone the repository and it's submodules.
```bash
git clone --recursive git@gitlab.com:Nathan675/home-automation-box-security.git homeautomation
cd homeautomation/sources/yocto
```

2. Configure Yocto
```bash
source oe-init-build-env rpi3-build

bitbake-layers remove-layer ../meta-poky
bitbake-layers remove-layer ../meta-yocto-bsp
bitbake-layers add-layer ../../meta-raspberrypi
bitbake-layers add-layer ../../meta-homeautomation
```
Edit `sources/yocto/rpi3-build/conf/local.conf`
```bash
#MACHINE ??= "qemux86"
MACHINE ?= "raspberrypi3"
#DISTRO ?="poky"
DISTRO ?= "homeautomation"
```

3. Set up mender  
Follow our [mender documentation](docs/mender.md)

4. Build 
```bash
bitbake core-image-homeautomation
```
