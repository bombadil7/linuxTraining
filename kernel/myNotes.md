Linux kernel sources can be pulled from 
```
~> git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```
Checking kernel log messages:
```
~> dmesg
or 
~> tail -f /var/log/syslog
```
USB serial communicator appears as `/dev/ttyUSB0`
BBB does not need to be powered on for this device to be detected.

Can check permissions for this device with `ls -l /dev/ttyUSB0`.  Owner root, and group dialout. Add yourself to this group:
```
~> sudo adduser $USER dialout
```
Need to log out and then back in to make it apparent.  Run `groups $USER` to make sure.
Start the serial connection:
```
picocom -b 115200 /dev/ttyUSB0
```
To exit picocom press `[Ctrl][a]` followed by `[Ctrl][x]`.


## Kernel Configuration
Use default OMAP2 config file in `arch/arm/configs`.
```
~> make ARCH=arm omap2plus_defconfig
~> make ARCH=arm menuconfig
~> make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- CONFIG_ROOT_NFS=y -j4 zImage
~> make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- CONFIG_ROOT_NFS=y -j4 dtbs
```

We generated two files: am335x-boneblack.dtb and zImage located in 
```
./arch/arm/boot/dts/am335x-boneblack.dtb
./arch/arm/boot/zImage
```

Copy them to host's /var/lib/tftpboot (can't access this location remotely.  
Have to copy to some location on the target, and then copy into /var/lib locally
with root access.

To check that U-Boot is working:
```
U-Boot > printenv
```

## Automate NFS Boot Process
```
U-Boot# setenv bootcmd 'setenv ipaddr 192.168.0.100; setenv serverip 192.168.0.1; setenv bootargs root=/dev/nfs rw ip=192.168.0.100 console=ttyO0 nfsroot=192.168.0.1:/home/bombadil/work/linuxTrain/kernel/modules/nfsroot; saveenv; tftp 0x81000000 zImage; tftp 0x82000000 am335x-boneblack.dtb; bootz 0x81000000 - 0x82000000'
U-Boot# saveenv
U-Boot# boot
```
Note: could also boot from usb - `usbboot` and copy files to eMMC and boot from there.  Need to research.

```
~> modinfo <modname>
~> modinfo <modpath>.ko
~> sudo insmod <modpath>.ko
~> sudo modprobe <module_name> # loads all dependent modules and then the module itself

lsmod - displayes loaded modules.  Compare with /proc/modules
sudo rmmod <module_name>
sudo modprobe -r <module_name> - Tries to remove the module and all dependent modules.
```

## Adding Module to Kernel Tree
tutorial [here](https://geekwentfreak-raviteja.rhcloud.com/blog/2010/10/24/adding-a-new-kernel-module-to-linux-source-tree/ "goodTutorial")

 - Copy module to `drivers/misc`
 - Add module to the file `drivers/misc/Kconfig`
 - Add a line to `drivers/misc/Makefile`
 ```
obj-$(CONFIG_HELLO_VERSION)+= hello_version.o
 ```
 - Run usual pre-compile sequence:
 ```
~> export CROSS_COMPILE=arm-linux-gnueabi-
~> export ARCH=arm
~> make clean
~> make omap2plus_defconfig
~> make menuconfig
 ```
 - Run xconfig and  check if it's available.
 - Finally compile this thing:
 ```
~> make -j4 zImage dtbs modules
 ```
 Note that CONFIG_ROOT_NFS=y must be set, but I assume it's been set in config file 
 already.

## Create Custom Device Tree
