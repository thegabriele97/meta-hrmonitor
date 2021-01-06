# meta-hrmonitor

This is a general overlay made only for learning purpose.

It offers two recipes, one includes a linux kernel module that implements a virtual PPG Sensor (a heart rate monitor), and the other one is an app that reads data from this device and computes the heart rate through the fft.

![#f03c15](https://via.placeholder.com/15/f03c15/000000?text=+) *This is tested only on RaspberryPi 1!*

### Add compatibility with other machines
![#c5f015](https://via.placeholder.com/15/c5f015/000000?text=+) *If you want to test the kernel module on another machine, **please add yours in COMPATIBLE_MACHINE** inside the file meta-hrmonitor/recipes-lkmodule/virtualppg/virtualppg.bb*

For example:

```
COMPATIBLE_MACHINE = '(raspberrypi|qemux86|qemuarm)'
```

## Quick Start

You can start with cloning this repository into your poky directory
```bash
git clone -b dunfell https://github.com/thegabriele97/meta-hrmonitor.git
```

Add the layer to your image by editing conf/bblayers.conf like this:
```
BBLAYERS ?= " \
  /home/gabri97/poky/meta \
  /home/gabri97/poky/meta-poky \
  /home/gabri97/poky/meta-yocto-bsp \
  /home/gabri97/poky/meta-openembedded/meta-oe \
  /home/gabri97/poky/meta-openembedded/meta-multimedia \
  /home/gabri97/poky/meta-openembedded/meta-python \
  /home/gabri97/poky/meta-openembedded/meta-networking \
  /home/gabri97/poky/meta-raspberrypi \
  /home/gabri97/poky/meta-hrmonitor \
  "
```

Now, register the app and the driver inside you image by editing your conf/local.conf and adding these lines

```
IMAGE_INSTALL_append = " virtualppg ppgreader"
KERNEL_MODULE_AUTOLOAD += "virtualppg"
```

Now launch bitbake, with this command or something similar
```bash
source oe-init-build-env build_dir
bitbake core-image-full-cmdline
```

*Enjoy!*
