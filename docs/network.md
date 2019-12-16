<!-- realized on 12/12/2019 by MALCOMBRE Nicolas -->

# Networks Configuration

## eth0 up

We need eth0 up from the start so to enable it from the start we will need to specified that we allow hotplug.  
So in `/etc/network/interfaces` we have to put :

```bash
#auto eth0
allow-hotplug eth0
```

## Dhcp

### Add configuration file to the distribution

In `homeautomation/src/meta-homeautomation/conf/distro/include` we created the file `networks.inc`

In the file `homeautomation/src/meta-homeautomation/conf/distro/homeautomation.conf`  
In the section `# include for features files` add

```bash
require conf/distro/include/networks.inc
```

### Add dhcp package

To complete `homeautomation/src/meta-homeautomation/conf/distro/include/networks.inc` file :

we need to have a dhcp client on the card to agree dhcp request so we add the [dhcp recipe](http://git.yoctoproject.org/cgit.cgi/poky/tree/meta/recipes-connectivity/dhcp?id=070f173bdc2e4e6704ae40d0ce54b22b0940c5c8) package.

```bash
IMAGE_INSTALL_append = " dhcp" 
```

