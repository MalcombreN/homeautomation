<!-- realized on 12/12/2019 by MALCOMBRE Nicolas -->
# Add configuration file to the distribution

In `homeautomation/src/meta-homeautomation/conf/distro/include`, create new file `server-openssh.inc`.  
Edit `homeautomation/src/meta-homeautomation/conf/distro/homeautomation.conf`, after `# include for features files`
```bash
require conf/distro/include/server-openssh.inc
```

# Add server

To complete `homeautomation/src/meta-homeautomation/conf/distro/include/server-openssh.inc` add [openssh server recipe](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/) package to use the basic openshh server and configuration.
```bash
IMAGE_INSTALL_append = " openssh"
```

## Add basic configuration

We obviously need to make our own configuration. We will override the [original configuration file](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/openssh/sshd_config) `sshd_config`.  
To override this file, we have to create a recipe with the same name and structure in `homeautomation/src/meta-homeautomation/`.
```bash
mkdir recipes-connectivity
cd recipes-connectivity
mkdir openshh
cd openshh
```
In this group of recipes we create the openssh recipe
```bash
mkdir openshh
cd openshh
```
In this recipe we create an openssh directory with two files `sshd_config` and `sshd.socket`.
```bash
mkdir openshh
touch openshh/sshd_config
touch openshh/sshd.socket
```

Copy [sshd_config](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/openssh/sshd_config) in `openshh/sshd_config` and [sshd.socket](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/openssh/sshd.socket) in `openshh/sshd.socket`.  
Then add a `.bbappend` file to override the basic recipe.
```bash
touch openssh_%.bbappend
```
Edit `openssh_%.bbappend` 
```bash
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
```
This command add the source directory called `${THISDIR}/${PN}`. `${THISDIR}` is `.` and `${PN}` is the *Package Name* equal to `openssh`. Finaly the extra path is `./openssh` where is stored `sshd_config`.

## Modification of basic configuration

Edit `sshd_config`
* port : 22 -> 49513 (any number between 49512 and 65535)
* PasswordAuthentication : yes -> no

And we add `HostKeyAlgorithms ssh-rsa` before `HostKey /etc/ssh/ssh_host_rsa_key` to use a rsa-key.  
For more information see [Add Hostkey](#add-hostkey).

Be careful to report the changes made to the default port in `sshd.socket`
* ListenStream=22 -> 49513 (or whichever value you took)

# Add Hostkey

Complete `sources/meta-homeautomation/conf/distro/include/server-openssh.inc` file to add box identity keys
```bash
IMAGE_INSTALL_append = " hostkeys"
```
Creation of the recipes pakage in `homeautomation/src/meta-homeautomation/recipes-connectivity`
```bash
mkdir hostkeys
cd hostkeys
touch hostkeys.bb
mkdir hostkeys
```

## Recipe creation

Now we have the recipe file `hostkeys.bb`. This recipe move `ssh_host_rsa_key` to `/etc/ssh` directory.  
Complete `hostkeys.bb`
```bash
DESCRIPTION = "add hostkeys to permit boxs authentification by the administrator"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
```

Then add keys in the root file system
```bash
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
```

Finaly create the package to install
```bash
FILES_${PN} += "\
	${sysconfdir}/ssh \
	${sysconfdir}/ssh/ssh_host_rsa_key.pub \
	${sysconfdir}/ssh/ssh_host_rsa_key \
    "
```

## Generate keys

To generate rsa key we use `ssh-keygen` in `hostkeys/hostkeys/` directory.
```bash
ssh-keygen -t rsa -f "ssh_host_rsa_key" -N ''
```

The `ssh_host_rsa_key.pub` should be copy in `.ssh/known_hosts` file.

# Add Keys

Complete `sources/meta-homeautomation/conf/distro/include/server-openssh.inc` to add the admin keys
```bash
IMAGE_INSTALL_append = " ssh-keys-server"
```

Creation of the recipes pakage in `homeautomation/src/meta-homeautomation/recipes-connectivity`
```bash
mkdir ssh-keys
cd ssh-keys
touch ssh-keys.bb
mkdir ssh-keys
```

## Recipe creation

The recipe `ssh-keys.bb` will move authorized ssh keys for connecting in `~/.ssh`
```bash
DESCRIPTION = "add ssh administrator's key to authorized keys"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
```

Then add the public key in the root file system
```bash
S = "${WORKDIR}"
SRC_URI = "\
	file://admin.pub \
"

ADMIN="homeautomationadmin"

do_install() {
        install -d ${D}/home/${ADMIN}/.ssh/
        install -m 0755 ${S}/admin.pub ${D}/home/${ADMIN}/.ssh/authorized_keys
}
```

Finaly create the package to install
```bash
PACKAGES += "${PN}-server"
FILES_${PN}-server += "\
	/home/${ADMIN}/.ssh/authorized_keys
"
```

## Adding administrator keys

Generate RSA administration keys using `ssh-keygen`
```bash
ssh-keygen -t rsa -f admin
```

Move your newly created public key to `ssh-keys/ssh-keys/admin.pub`, where it'll be used to verify your identity during SSH handshake.  
Copy both of the keys (`admin.pub` and `admin`) in your `~/.ssh` directory, which will be used to connect by SSH to a remote PC.
