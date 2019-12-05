<!-- realized on 4/12/2019 by MALCOMBRE Nicolas -->

# SSH server

## Configuration file

In `homeautomation/src/meta-homeautomation/conf/distro/include` we created the file `server-openssh.inc`

To complete `server-openssh.inc` file :


Add [openssh server recipe](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/) to use the basic openshh server and configuration 

```bash
IMAGE_INSTALL_append = " openssh" 
```

## Add configuration file to the distribution

In the file `homeautomation/src/meta-homeautomation/conf/distro/homeautomation.conf`

In the section `# include for features files` add

```bash
require conf/distro/include/server-openssh.inc
```

## Modification of basic configuration

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
In this recipe we create an openssh directory with a basic copy of `sshd_config`

```bash
mkdir openshh
touch openshh/sshd_config
```
Copy [sshd_config](https://git.yoctoproject.org/cgit.cgi/poky/plain/meta/recipes-connectivity/openssh/openssh/sshd_config) in `openshh/sshd_config`.

Then add a .bbappend file to overrride the basic recipe.

```bash
touch openssh_7.9p1.bbappend
```

In `openssh_7.9p1.bbappend` write 

```bash
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
```

This commande add to the openssh recipe the source directory called `${THISDIR}/${PN}`. `${THISDIR}` is `.` and `${PN}` is the *Package Name* egual as *Recipe Name* "openssh". Finaly the extra path is `./openssh` where is stored `sshd_config`.









