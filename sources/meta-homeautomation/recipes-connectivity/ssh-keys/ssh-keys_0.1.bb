DESCRIPTION = "add ssh administrator's key to authorized keys"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "\
file://ssh_host_rsa_key \
file://ssh_host_rsa_key.pub \
file://keys.pub \
          "
S = "${WORKDIR}"

ADMIN1="homeautomationadmin"

do_install() {
        install -d ${D}/home/${ADMIN1}/.ssh/
        install -m 0755 ${S}/keys.pub ${D}/home/${ADMIN1}/.ssh/authorized_keys
}

PACKAGES += "${PN}-server"

FILES_${PN}-server += "\
	/home/${ADMIN1}/.ssh/authorized_keys \
	"

