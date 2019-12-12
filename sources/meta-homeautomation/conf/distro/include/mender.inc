IMAGE_INSTALL_append = " install-box-key"

RPI_USE_U_BOOT = "1"
MENDER_PARTITION_ALIGNMENT = "4194304"
MENDER_BOOT_PART_SIZE_MB = "40"
IMAGE_INSTALL_append = " kernel-image kernel-devicetree"
IMAGE_FSTYPES_remove += " rpi-sdimg"

#######################################
#####		MENDER COMMON		#######
#######################################

# This really saves a lot of disk space!
INHERIT += "rm_work"

# The name of the disk image and Artifact that will be built.
# This is what the device will report that it is running, and different updates
# must have different names because Mender will skip installation of an Artifact
# if it is already installed.
MENDER_ARTIFACT_NAME = "release-test"

MENDER_UPDATE_POLL_INTERVAL_SECONDS = "5"
MENDER_RETRY_POLL_INTERVAL_SECONDS = "30"
MENDER_INVENTORY_POLL_INTERVAL_SECONDS = "100"

INHERIT += "mender-full"

DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
VIRTUAL-RUNTIME_initscripts = ""

# Build for Hosted Mender
#
# To get your tenant token:
#    - log in to https://hosted.mender.io
#    - click your email at the top right and then "My organization"
#    - press the "COPY TO CLIPBOARD"
#    - assign content of clipboard to MENDER_TENANT_TOKEN
#
#MENDER_SERVER_URL = "https://hosted.mender.io"
#MENDER_TENANT_TOKEN = ""

# Build for Mender demo server
#
# https://docs.mender.io/getting-started/create-a-test-environment
#
# Uncomment below and update IP address to match the machine running the
# Mender demo server
MENDER_DEMO_HOST_IP_ADDRESS = "192.168.1.1"

# Build for Mender production setup (on-prem)
#
# https://docs.mender.io/artifacts/building-for-production
#
# Uncomment below and update the URL to match your configured domain
# name and provide the path to the generated server.crt file.
#
# NOTE! It is recommend that you provide below information in your custom
# Yocto layer and this is only for demo purposes. See linked documentation
# for additional information.
MENDER_SERVER_URL = "https://docker.mender.io"
#FILESEXTRAPATHS_prepend_pn-mender := "<DIRECTORY-CONTAINING-server.crt>:"
#SRC_URI_append_pn-mender = " file://server.crt"

# Mender storage configuration
#
# More details on these variables is available at
#    https://docs.mender.io/devices/yocto-project/partition-configuration#configuring-storage
#
# Also, please be sure to check other config files as other
# layers, config fragments, etc may attempt to set values
# for specific platforms.  Using "bitbake -e <image-name>"
# can help determine which files are setting these values
# in a given configuration.
#
MENDER_STORAGE_TOTAL_SIZE_MB = "2048"
# MENDER_BOOT_PART_SIZE_MB = "16"
MENDER_DATA_PART_SIZE_MB = "1024"
MENDER_STORAGE_DEVICE = "/dev/mmcblk0"
# MENDER_BOOT_PART = "${MENDER_STORAGE_DEVICE_BASE}1"
# MENDER_DATA_PART = "${MENDER_STORAGE_DEVICE_BASE}4"
# MENDER_ROOTFS_PART_A = "${MENDER_STORAGE_DEVICE_BASE}2"
# MENDER_ROOTFS_PART_B = "${MENDER_STORAGE_DEVICE_BASE}3"
