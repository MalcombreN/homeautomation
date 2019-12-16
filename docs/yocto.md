<!-- realized on 3/12/2019 by MALCOMBRE Nicolas -->

# Generalized setup

## GitLab setup 

Create a work directory
```bash
$ mkdir homeautomation
$ cd homeautomation
```

Creation of a git repository.  
```bash
$ git init
```

To remotely create a git repository you must have a commit.
```bash
$ touch README.md
$ git status
$ git commit -am "first commit"
```

Push the commit in a new git repository.  
Replace `https://gitlab.com/namespace/projectname.git` with your remote repository.
```bash
$ git push --set-upstream  https://gitlab.com/namespace/projectname.git master
```

Creation of a directory for sources and documentation
```bash
$ mkdir src doc
```

Download `warrior` version of `poky` from yocto project in `src` directory.  
Then declare it as a **git submodule**. The `git submodule` command automatically download it.
```bash
$ cd src
$ git submodule add -b warrior https://git.yoctoproject.org/git/poky yocto
```

Declare a git submodule for the layer `meta-raspberrypi` in sources directory.  
```bash
$ git submodule add -b warrior https://git.yoctoproject.org/git/meta-raspberrypi
```

## Yocto layers setup

Creation of the `homeautomation` layer
```bash
$ cd yocto
$ source oe-init-build-env rpi3-build
$ bitbake-layers create-layer ../../meta-homeautomation
```

Layers gestion
```bash
$ bitbake-layers remove-layer ../meta-poky
$ bitbake-layers remove-layer ../meta-yocto-bsp
$ bitbake-layers add-layer ../../meta-raspberrypi
$ bitbake-layers add-layer ../../meta-homeautomation
$ bitbake-layers show-layers
```

## meta-homeautomation setup

```bash
$ cd ../../meta-homeautomation/
$ rm -r recipes-example/
```

Creation of the configuration file for the distribution
```bash
$ mkdir conf/distro
$ touch conf/distro/homeautomation.conf
```

And add the following content
```bash
DISTRO_NAME = "homeautomation"
DISTRO_VERSION = "1.0"

# enable the uart for debug
ENABLE_UART = "1"

# put systemd as init program
DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"

# include for features files 
```

Creation of the directory where configuration of additional distribution features will be
```bash
$ mkdir conf/distro/include
```

Creation of the core-image called `core-image-homeautomation`
```bash
$ mkdir -p recipes-core/images
$ touch recipes-core/images/core-image-homeautomation.bb
```

In this file we have to put
```bash
SUMMARY = "A small image for a homeautomation box."
LICENSE = "MIT"

inherit core-image
```

## First image creation

Go in yocto build configuration 
```bash
$ cd ../yocto/rpi3-build/conf
```

Edit `local.conf` file
Comment the `MACHINE` variable and add the new one 
```bash
#MACHINE ??= "qemux86"
MACHINE ?= "raspberrypi3"
```

Comment the `DISTRO` variable and add the new one
```bash
#DISTRO ?="poky"
DISTRO ?= "homeautomation"
```

We can now create the image
```bash
$ bitbake core-image-homeautomation
```
