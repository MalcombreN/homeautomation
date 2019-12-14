DESCRIPTION = "add ssh administrator's key to authorized keys"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "\
	file://admin.pub \
"

ADMIN1="homeautomationadmin"


do_install() {
	# admins public keys
        install -d ${D}/home/${ADMIN1}/.ssh/
        install -m 0755 ${WORKDIR}/admin.pub ${D}/home/${ADMIN1}/.ssh/authorized_keys
}

PACKAGES += "${PN}-server"

FILES_${PN}-server += "\
	/home/${ADMIN1}/.ssh/authorized_keys \
    "

