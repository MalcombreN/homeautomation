DESCRIPTION = "ssh private & public key packages"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " file://serv_key_rsa.pub \
          "
S = "${WORKDIR}"

USER="homeautomationadmin"

do_install() {
    install -d ${D}/home/${USER}/.ssh/    
    install -m 0755 ${S}/serv_key_rsa.pub ${D}/home/${USER}/.ssh/authorized_keys
}

PACKAGES += "${PN}-client ${PN}-server"

FILES_${PN}-client += "/home/${USER}/.ssh/serv_key_rsa.pub"
FILES_${PN}-server += "/home/${USER}/.ssh/authorized_keys"
