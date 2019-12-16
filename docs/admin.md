<!-- realized on 3/12/2019 by MALCOMBRE Nicolas -->

# Configuration file

In `homeautomation/src/meta-homeautomation/conf/distro/include` we created the file `add-admin.inc`.  
We want to use [sudo recipe](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-extended/sudo/) to allow the administrator to run commands as sudo
```bash
IMAGE_INSTALL_append = " sudo"
```

To use yocto variable to add users we have to use the bbclasse `extrausers.bbclass` from [openembedded-core](https://github.com/openembedded/openembedded-core/blob/master/meta/classes/extrausers.bbclass)
```bash
inherit extrausers
```

## Credentials

Hash the password using `mkpasswd` so a reinforced security
```bash
mkpasswd -m SHA-512
```
On the generated hash put `\\\` before all `$`
```bash
USER = "homeautomationadmin"
PASSWORD = '\\\$6\\\$5mtzn6dZCCyrhS\\\$hl01KxfXRmI7yRJp1vNtgp7gAr69SJWh6c4rPyJhwCSvVH02UYlMHeNiDt0FZpRGDi.9nADwm6hssW1OW9XZ6/'
```
Then complete `EXTRA_USERS_PARAMS` variable
```bash
EXTRA_USERS_PARAMS = " \
	usermod -L root; \
	useradd -g sudo -p "${PASSWORD}" ${USER}; \
```
`usermod -L root` locks the root account authentification  
`useradd -g sudo -p "${PASSWORD}" ${USER}` add the USER `homeautomationadmin` to the `sudo` group. 

At this time the group sudo is a simple group. To allow adim rights we have to create a process that will be execute after the root file system's creation.
```bash
# process adding sudo group to sudoers
update_sudoers(){
    sed -i 's/# %sudo/%sudo/' ${IMAGE_ROOTFS}/etc/sudoers
}
```
`sed` will uncoment by replacing `# %sudo` by `%sudo` in the `/etc/sudoers` file of the root file system.

To execute that process after the root file system's creation we have to put the name in the `ROOTFS_POSTPROCESS_COMMAND` variable
```bash
ROOTFS_POSTPROCESS_COMMAND += "update_sudoers;"
```

# Add configuration file to the distribution

Edit `homeautomation/src/meta-homeautomation/conf/distro/homeautomation.conf`, section `# include for features files`
```bash
require conf/distro/include/add-admin.inc
```






















