# configure a static eth0 network by overwriting the original recipes
# in meta-homeautomation/recipes-connectivity/init-ifupdown

IMAGE_INSTALL_append = " ifupdown"
