<!-- realized on 8/12/2019 by MALCOMBRE Nicolas -->

# SSH server

## Add server

In `homeautomation/src/meta-homeautomation/conf/distro/include` we created the file `server-openssh.inc`

To complete `server-openssh.inc` file :


Add [openssh server recipe](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/) to use the basic openshh server and configuration 

```bash
IMAGE_INSTALL_append = " openssh" 
```

### Add configuration file to the distribution

In the file `homeautomation/src/meta-homeautomation/conf/distro/homeautomation.conf`

In the section `# include for features files` add

```bash
require conf/distro/include/server-openssh.inc
```

### Add basic configuration

We obviously need to make our own configuration so wee have to override the [original configuration file](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/openssh/sshd_config) called `sshd_config`.

To override this file we have to recipe with the same name and structure.

Creation of the recipes pakage in `homeautomation/src/meta-homeautomation/`

```bash
mkdir recipes-connectivity
cd recipes-connectivity
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

Then add a .bbappend file to override the basic recipe.

```bash
touch openssh_7.9p1.bbappend
```

In `openssh_7.9p1.bbappend` write 

```bash
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
```

This commande add to the openssh recipe the source directory called `${THISDIR}/${PN}`. `${THISDIR}` is `.` and `${PN}` is the *Package Name* egual as *Recipe Name* "openssh". Finaly the extra path is `./openssh` where is stored `sshd_config`.

### Modification of basic configuration

In `sshd_config` we change 

- port : 22 -> 49513
- MaxAuthTries : 6 -> 3
- MaxSessions : 10 -> 1
- PasswordAuthentication : yes -> no

And we add `HostKeyAlgorithms ssh-rsa` before `HostKey /etc/ssh/ssh_host_rsa_key` to signifie that we want to send to the ssh-client a rsa-key. For more information see **Add Keys** section and **Add Hostkey** [subsection](anchor).

If you modify the default port in `sshd_config` :
- Take a new port between 49 152 and 65 535.
- Modify `sshd.socket`.
	- ListenStream=22 -> *newport*



## Add Keys

Complete `homeautomation/src/meta-homeautomation/conf/distro/include/server-openssh.inc` file to add the recipe to use our personals keys :

```bash
IMAGE_INSTALL_append = " ssh-keys-server"
```

Creation of the recipes pakage in `homeautomation/src/meta-homeautomation/recipes-connectivity`

```bash
mkdir ssh-keys
cd ssh-keys
touch ssh-keys_0.1.bb
mkdir ssh-keys-0.1
```
### Recipe creation

Now we have the recipe file `ssh-keys_0.1.bb` witch have to put in a .ssh directory all authorized keys.

So now complete `ssh-keys_0.1.bb` :

```bash
DESCRIPTION = "add ssh administrator's key to authorized keys"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
```

Then add the public key in the root file system

```bash
S = "${WORKDIR}"
SRC_URI = "file://keys.pub \
          "

ADMIN1="homeautomationadmin"

do_install() {
        install -d ${D}/home/${ADMIN1}/.ssh/
        install -m 0755 ${S}/keys.pub ${D}/home/${ADMIN1}/.ssh/authorized_keys
}
```

Finaly create the package to install

```bash
PACKAGES += "${PN}-server"
FILES_${PN}-server += "/home/${ADMIN1}/.ssh/authorized_keys"
```
### generate keys

To generate rsa key we use **ssh-keygen**

```bash
ssh-keygen -t rsa -f rpi3bplus
```

### adding administrator keys

To add your own keys create in `ssh-keys/ssh-keys-0.1` a file called `keys.pub` where public keys will be. 
The previous generated **public** key *rpi3bplus.pub* have to be copy in `keys.pub` file.
The previous generated **private** key *rpi3bplus* have to be put in the administrator's pc in the directory `~/.ssh/`.

### [](#anchor)Add Hostkey










