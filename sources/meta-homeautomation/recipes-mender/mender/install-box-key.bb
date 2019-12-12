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
