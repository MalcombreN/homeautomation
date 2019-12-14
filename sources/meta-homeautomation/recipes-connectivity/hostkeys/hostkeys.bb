DESCRIPTION = "add hostkeys to permit boxs authentification by the administrator"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "\
	file://ssh_host_rsa_key.pub \
	file://ssh_host_rsa_key \
"

do_install() {
	# admins public keys
        install -d ${D}${sysconfdir}/ssh/
        install -m 0644 ${WORKDIR}/ssh_host_rsa_key.pub ${D}${sysconfdir}/ssh/ssh_host_rsa_key.pub
	install -m 0600 ${WORKDIR}/ssh_host_rsa_key ${D}${sysconfdir}/ssh/ssh_host_rsa_key
}

FILES_${PN} += "\
	${sysconfdir}/ssh \
	${sysconfdir}/ssh/ssh_host_rsa_key.pub \
	${sysconfdir}/ssh/ssh_host_rsa_key \
    "

