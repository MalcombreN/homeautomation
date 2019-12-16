# Mender.io

# Docker Engine

[Documentation](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

## Remove old versions

```bash
$ sudo apt remove docker docker-engine docker.io containerd runc
```
None of these packages should be installed

## Setup repository

1. Install pre-requisites packages
```bash
$ sudo apt update
$ sudo apt install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
```

2. Add PGP Docker key
```bash
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
$ sudo apt-key fingerprint 0EBFCD88
pub   rsa4096 2017-02-22 [SCEA]
      9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88
uid           [ unknown] Docker Release (CE deb) <docker@docker.com>
sub   rsa4096 2017-02-22 [S]
```

3. Add the docker repository
```bash
$ sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
```

## Installing

1. Update `apt` package index
```bash
$ sudo apt-get update
```

2. Install docker engine
```bash
$ sudo apt-get install docker-ce docker-ce-cli containerd.io
```

3. Verifiy your installation with `hello-world` image
```bash
$ sudo docker run hello-world
```

# Docker compose

[Documentation](https://docs.docker.com/compose/install/)

Fetch the current release
```bash
$ sudo curl -L "https://github.com/docker/compose/releases/download/1.25.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose

$ sudo chmod +x /usr/local/bin/docker-compose
```

If `docker-compose` isn't in the path
```bash
$ sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose
```

Test the installation
```bash
$ docker-compose --version
docker-compose version 1.25.0, build 1110ad01
```

# Server

[Documentation](https://docs.mender.io/2.1/administration/production-installation)

Pre-requisites
* Ubuntu 18.04
* Git
* Docker engine >= 17.03
* Docker compose >= 1.6
* 10GB disk space, 4GB RAM

1. Fetch the [server sources](https://github.com/mendersoftware/integration)
```bash
$ cd homeautomation
$ mkdir server && cd server
$ git submodule add -b 2.1.1 https://github.com/mendersoftware/integration
$ cd integration/production
$ cp config/prod.yml.template config/prod.yml
```

2. Download docker images
```bash
# required to manipulate docker scripts
$ sudo usermod -a -G docker <user>
# required by server scripts
$ sudo apt install jq
# download the docker images
$ ./run pull
```

3. Modify network configuration in `/etc/hosts`
```bash
127.0.0.1 docker.mender.io s3.docker.mender.io
```

## Certificates

[Documentation](https://docs.mender.io/2.2/administration/certificates-and-keys)

Mender uses a micro-services architecture where multiple components are used to make a global task  
Some of those components will talk with the mender client on the box and thus must have their own certificates

Using the provided script, generate the certificates
```bash
$ CERT_API_CN=docker.mender.io CERT_STORAGE_CN=s3.docker.mender.io ../keygen
# all the certificates are in the keys-generated directory
$ ls -R keys-generated
```

## Storage

Enable persistent storage for logs & DBs inside docker containers
```bash
$ docker volume create --name=mender-artifacts
$ docker volume create --name=mender-db
$ docker volume create --name=mender-elasticsearch-db
$ docker volume create --name=mender-redis-db
# inspect a particular volume
$ docker volume inspect --format '{{.Mountpoint}}' mender-artifacts
/var/lib/docker/volumes/mender-artifacts/_data
```

## Matching configuration

Remaining renaming hostnames and generating credentials in `configs/prod.yml`

1. storage-proxy
Changes `aliases` to match our domain name
```bash
    ...
    storage-proxy:
        networks:
            mender:
                aliases:
                    - s3.docker.mender.io
    ...
```

2. minio
Generate `minio` `secret key` with `pwgen`
```bash
$ pwgen 16 1
sahzahPh5eiquohz
```

```bash
    ...
    minio:
        environment:
            # access key
            MINIO_ACCESS_KEY: mender-deployments
            # secret
            MINIO_SECRET_KEY: sahzahPh5eiquohz
    ...
```

3. mender-deployments
`DEPLOYMENTS_AWS_AUTH_SECRET` should be the same as `MINIO_SECRET_KEY`
```bash
    ...
    mender-deployments:
        ...
        environment:
            DEPLOYMENTS_AWS_AUTH_KEY: mender-deployments
            DEPLOYMENTS_AWS_AUTH_SECRET: sahzahPh5eiquohz
            DEPLOYMENTS_AWS_URI: https://s3.docker.mender.io:9000
    ...
```

4. mender-api-gateway
```bash
    ...
    mender-api-gateway:
        ...
        environment:
            ALLOWED_HOSTS: docker.mender.io
    ...
```

5. mender-device-auth
```bash
    ...
    mender-device-auth:
        ...
        environment:
            DEVICEAUTH_MAX_DEVICES_LIMIT_DEFAULT: 15
    ...
```

## Starting the server

1. First we need to run the server
```bash
$ ./run up -d
```

2. Then create our user
```bash
$ ./run exec mender-useradm /usr/bin/useradm create-user --username=homeautomation@example.com --password=homeautomation
```

3. Finally navigate to `https://docker.mender.io` and log in

# Client

[Documentation](https://docs.mender.io/2.2/getting-started/on-premise-installation/install-the-mender-client)

[Documentation](https://docs.mender.io/2.2/artifacts/yocto-project/building-for-production)

[RaspberryPi 3b+ tuto](https://hub.mender.io/t/raspberry-pi-3-model-b-b/57)

## Install mender repository and dependencies
```bash
$ cd sources
$ git submodule add -b warrior https://github.com/openembedded/meta-openembedded.git meta-openembedded
$ git submodule add -b warrior https://github.com/mendersoftware/meta-mender.git meta-mender
```

## Configuration

We will integrate the mender client into our Yocto build system

1. Append the [common configuration](https://github.com/mendersoftware/meta-mender-community/blob/warrior/templates/local.conf.append) for mender to `sources/meta-homeautomation/conf/distro/include/mender.inc`
```bash
INHERIT += "rm_work"

MENDER_ARTIFACT_NAME = "release-1"

MENDER_UPDATE_POLL_INTERVAL_SECONDS = "5"
MENDER_RETRY_POLL_INTERVAL_SECONDS = "30"
MENDER_INVENTORY_POLL_INTERVAL_SECONDS = "100"

INHERIT += "mender-full"

DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
VIRTUAL-RUNTIME_initscripts = ""

MENDER_DEMO_HOST_IP_ADDRESS = "192.168.1.1"
MENDER_SERVER_URL = "https://docker.mender.io"

MENDER_STORAGE_TOTAL_SIZE_MB = "2048"
# MENDER_BOOT_PART_SIZE_MB = "16"
MENDER_DATA_PART_SIZE_MB = "1024"
MENDER_STORAGE_DEVICE = "/dev/mmcblk0"
```

2. Append [board configuration](https://github.com/mendersoftware/meta-mender-community/blob/warrior/meta-mender-raspberrypi/templates/local.conf.append) to `sources/meta-homeautomation/conf/distro/include/mender.inc`
```bash
MACHINE ?= "raspberrypi3"

RPI_USE_U_BOOT = "1"
MENDER_PARTITION_ALIGNMENT = "4194304"
MENDER_BOOT_PART_SIZE_MB = "40"
IMAGE_INSTALL_append = " kernel-image kernel-devicetree"
# Mender uses .sdimg images with it's own partitionning
IMAGE_FSTYPES_remove += " rpi-sdimg"

MENDER_FEATURES_ENABLE_append = " mender-uboot mender-image-sd"
MENDER_FEATURES_DISABLE_append = " mender-grub mender-image-uefi"
```

3. Add this file to `sources/meta-homeautomation/conf/distro/homeautomation.conf`
```bash
require include/mender.inc
```

4. Add usefull layers
```bash
$ cd yocto
$ source oe-init-build-env rpi3-build
$ bitbake-layers add-layer ../../meta-openembedded/meta-oe
$ bitbake-layers add-layer ../../meta-openembedded/meta-python
$ bitbake-layers add-layer ../../meta-openembedded/meta-networking
$ bitbake-layers add-layer ../../meta-openembedded/meta-multimedia
$ bitbake-layers add-layer ../../meta-mender/meta-mender-core
$ bitbake-layers add-layer ../../meta-mender/meta-mender-raspberrypi
```

5. Configure network

```bash
$ mkdir -p sources/meta-homeautomation/recipes-core/base-files
```
Create recipe `sources/meta-homeautomation/recipes-core/base-files/base-files_%.bbappend`
```bash
do_install_append() {
    if [ -n "${MENDER_DEMO_HOST_IP_ADDRESS}" ]; then
        echo "${MENDER_DEMO_HOST_IP_ADDRESS} docker.mender.io s3.docker.mender.io" >> ${D}${sysconfdir}/hosts
	fi
}
```

## Certificates

Override `meta-mender/mender` recipe to add the server certificate
```bash
$ mkdir -p sources/meta-homeautomation/meta-mender/mender/files
$ cp server/integration/production/keys-generated/certs/server.crt sources/meta-homeautomation/meta-mender/mender/files
```

In `sources/meta-homeautomation/meta-mender/mender/mender_%.bbappend`
```bash
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI_append = " file://server.crt"
```

# Sign OTA updates

[Documentation](https://docs.mender.io/2.2/artifacts/signing-and-verification)
[Help](https://docs.mender.io/2.2/artifacts/yocto-project/variables#mender_artifact_verify_key)

## Build system

Generate the build system key pair
```bash
$ cd server/integration/production/keys-generated
$ mkdir artifact
$ cd artifact
$ openssl genpkey -algorithm RSA -out private.key -pkeyopt rsa_keygen_bits:3072
$ openssl rsa -in private.key -out private.key
$ openssl rsa -in private.key -out public.key -pubout
```

Install the `mender-artifact` utility
```bash
$ cd server
$ wget https://d1b0l86ne08fsf.cloudfront.net/mender-artifact/3.2.1/linux/mender-artifact
$ chmod +x mender-artifact
```

## Mender client

Override `meta-mender/mender` recipe to add the artifact signing public key
```bash
$ mkdir -p sources/meta-homeautomation/meta-mender/mender/files
$ cp server/integration/production/keys-generated/artifact/public.key sources/meta-homeautomation/meta-mender/mender/files/artifact-verify-key.pem
```

In `sources/meta-homeautomation/meta-mender/mender/mender_%.bbappend`
```bash
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI_append = " file://artifact-verify-key.pem"
```

## Signing an artifact

```bash
cd server
# create symlinks to simplify the task
$ ln -s sources/yocto/rpi-build/tmp-glibc/deploy/images/raspberrypi3/core-image-homeautomation.mender image.mender
$ ln -s server/integration/keys-generated/artifact/private.key artifact.pk
$ ln -s server/integration/keys-generated/artifact/public.key artifact.pub
# sign image
$ ./mender-artifact sign image.mender -k artifact.pk -o signed.mender
# verifying signature
$ ./mender-artifact validate signed -k artifact.pub
```

# Device authentication

[Documentation](https://docs.mender.io/2.2/server-integration/preauthorizing-devices)

For the server to be able to recognize our box, it will need both
it's unique MAC address and given public key.

## Server

1. Get the MAC address of the interface used to communicate with the server
For this example, we'll use eth0
```bash
$ cat /sys/class/net/eth0/address
# should output the mac address in the form
b8:27:eb:f9:76:5c
```

2. Generate a key pair for the box
We will use the provided script from mender
```bash
$ cd server/integration/production/keys-generated
$ mkdir box
$ cd box
$ wget https://raw.githubusercontent.com/mendersoftware/mender/2.1.2/support/keygen-client
$ chmod +x keygen-client
$ ./keygen-client
$ ls keys-client-generated
private.key public.key
```

3. Use the server API to preauthorize your device

[Documentation](https://docs.mender.io/2.2/server-integration/using-the-apis#set-up-shell-variables-for-curl)

```bash
$ MENDER_SERVER_URI='https://docker.mender.io'
$ MENDER_SERVER_USER='homeautomation@example.com'
$ JWT=$(curl -k -X POST -u $MENDER_SERVER_USER $MENDER_SERVER_URI/api/management/v1/useradm/auth/login)
$ echo $JWT
```

Try calling the API
```bash
$ curl -k -H "Authorization: Bearer $JWT" $MENDER_SERVER_URI/api/management/v1/useradm/users | jq '.'
```

4. Make sure server don't know your device
```bash
$ curl -k -H "Authorization: Bearer $JWT" $MENDER_SERVER_URI/api/management/v2/devauth/devices | jq '.' | grep 'b8:27:eb:f9:76:5c'
```

If you find a match you must delete their entries first
```bash
$ curl -k -H "Authorization: Bearer $JWT" -X DELETE $MENDER_SERVER_URI/api/management/v2/devauth/devices/{devID}
```

5. Set the preauthorization

```bash
$ DEVICE_IDENTITY_JSON_OBJECT_STRING='{"mac":"b8:27:eb:f9:76:5c"}'
$ DEVICE_PUBLIC_KEY="$(cat keys-client-generated/public.key | sed -e :a  -e 'N;s/\n/\\n/;ta')"
$ curl -k -H "Authorization: Bearer $JWT" -H "Content-Type: application/json" -X POST -d "{ \"identity_data\" : $DEVICE_IDENTITY_JSON_OBJECT_STRING, \"pubkey\" : \"$DEVICE_PUBLIC_KEY\" }" $MENDER_SERVER_URI/api/management/v2/devauth/devices
```

To verify if it worked
```bash
$ curl -k -H "Authorization: Bearer $JWT" $MENDER_SERVER_URI/api/management/v2/devauth/devices | jq '.'
```
Your device should appear with the `preauthorized` status

## Client

We need to copy our generated private key into the box file system  
The private key must reside in `/data/mender/mender-agent.pem` (on the data partition)

1. Create the file `sources/meta-homeautomation/recipes-mender/mender/install-box-key.bb`
```bash
DESCRIPTION = "Copy box private key"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI = "file://mender-agent.pem"

FILES_${PN} += "/data/mender/mender-agent.pem"

S = "${WORKDIR}"

do_install() {
    install -d ${D}/data/mender/
    install -m 0644 mender-agent.pem ${D}/data/mender/
}
```

2. Copy the private key in the `files` directory
```bash
$ mkdir -p sources/meta-homeautomation/recipes-mender/mender/files/
$ cp server/integration/production/keys-generated/box/keys-client-generated/private.key sources/meta-homeautomation/recipes-mender/mender/files/mender-agent.pem
```

3. Add the recipe to the configuration in `sources/meta-homeautomation/conf/distro/include/mender.inc`
```bash
IMAGE_INSTALL_append = " install-box-key"
```

# Test

1. Build the `.sdimg` image and `.mender` artifact
```bash
$ bitbake core-image-homeautomation
```
2. [Flash](flash.md) it on the card
3. Log in using [UART](flash.md#connect-to-the-box-using-uart)

## Configure network

If you are using point to point connection without DHCP or internet server
you will need to configure the network manually

1. On the raspberry Pi
```bash
# disable network time protocol
$ timedatectl set-ntp 0
# replace YYYY-MM-DD hh:mm:ss with the current date at UTC time
$ timedatectl set-time "YYYY-MM-DD hh:mm:ss"
# set eth0 ip configuration
$ ifconfig eth0 192.168.1.10 netmask 255.255.255.0 up
```

2. On the server
```bash
$ ip addr add 192.168.1.1/24 dev eth0
$ ip link dev eth0 up
```

3. Try the connection
On the raspberry Pi
```bash
$ ping docker.mender.io
$ ping 192.168.1.1
```
On the server
```bash
$ ping 192.168.1.10
```

4. Using an internet browser
Navigate to `https://docker.mender.io`  
Connect with your credentials  
Go to the `Devices` tab  
Wait for you device to synchronize

# Release

1. Change release name in `sources/meta-homeautomation/conf/distro/include/mender.inc`
```bash
MENDER_ARTIFACT_NAME = "release-X"
```

2. Build the release
```bash
$ cd sources/yocto/rpi-build
$ bitbake core-image-homeautomation
```

3. [Sign the release](#signing-an-artifact)  

4. Upload it to the server
In `releases` tab drag the signed artifact in the browser

5. Deploy it
Press the `create deployment with this release` button in the `releases` tab

6. Update status
In `deployments` tab you can see the deployment status  

If you don't use a DHCP server, you will need to re-log into the raspberry Pi from the UART to [reconfigure the network](#configure-network).

# Teardown

Stop the server
```bash
$ server/integration/production/run stop
```