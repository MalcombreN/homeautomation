# install openssh and add our sshd_config by overwriting the original recipes
# in meta-homeautomation/recipes-connectivity/openssh
IMAGE_INSTALL_append = " openssh"
