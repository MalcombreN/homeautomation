IMAGE_INSTALL_append = " sudo"

inherit extrausers

# disable root authentification
# add homeautomationadmin user with a SHA512 password
# don't forget to put \\\ before all $ of your SHA512 password 
#USER = "homeautomationadmin"
USER = "homeautomation"
#PASSWORD = '\\\$6\\\$5mtzn6dZCCyrhS\\\$hl01KxfXRmI7yRJp1vNtgp7gAr69SJWh6c4rPyJhwCSvVH02UYlMHeNiDt0FZpRGDi.9nADwm6hssW1OW9XZ6/'
PASSWORD = '\\\$6\\\$.qXcHOzK7hBV\\\$t/E4wqYK2Bh.PFYeZogIf5KAExJBJkRo77mp2hW7QCaSRgB5/OgSY8l.lfJqdtzcSbJmvDrCDrrxYTU1LHuUs/'

EXTRA_USERS_PARAMS = " \
	usermod -L root; \
	useradd -g sudo -p "${PASSWORD}" ${USER}; \
"

# process adding sudo group to sudoers
update_sudoers(){
    sed -i 's/# %sudo/%sudo/' ${IMAGE_ROOTFS}/etc/sudoers
}

ROOTFS_POSTPROCESS_COMMAND += "update_sudoers;"
