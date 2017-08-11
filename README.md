# ENPH 253 ROBOT 2017

Software stack for the Magic School Bot. For more information please visit our [website](https://walkervilleelementary.github.io)

# Table of Content

- [Installation](#installation)
- [Compile](#compile)

-----

## Installation

Please make sure cmake and avr-gcc-toolchain is availible. Refer to [this](http://avr-eclipse.sourceforge.net/wiki/index.php/The_AVR_GCC_Toolchain) for help with avr-gcc-toolchain.

Please make sure `programmer_config.cmake` can be found in the same directory as `CMakeLists.txt`. Example of file:

```
set(PROG_TYPE wiring)
set(PROG_CHIP m128)
set(PROG_BAUD 115200)
set(PROG_PORT /dev/Tinah) // replace this with the port the Tinah is connected to.
```

*for window users the port would be something like COM3 or COM#*

### For Unix users only

The Tinah might not show up as the same device each time. So it will be convenient to have a [udev rule](https://wiki.debian.org/udev). This would ensure that the Tinah would show up at the same port each time. Beware of plugging in 2 Tinahs at the same time. Example of file:

```
# ENPH 253 ROBOT - wiring TINAH
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", MODE="0666" SYMLINK+="tinah", GROUP="dialout"
```

This would need to be placed in `/etc/udev/rules.d` with a file name like `40-tinah.rules`. A reboot might be needed for the rule to work properly. As well, it might be necessary for the user to be in the group `dialout` to communicate with the Tinah. `sudo usermod -a -G dialout yourUserName` should allow you be in the `dialout` group if you are not already.

---

## Compile

```
// inside the repo
mkdir build  // make a directory called build
cd  build    // go into the build directory
cmake ..     // run cmake
make flash   // compile the code and flash onto the tinah
```

### Window Users

It might be necessary to use [Ninja](https://ninja-build.org/) instead of `make`. So make sure `Ninja` is in your path variable and replace the last 2 command with:

```
cmake -G "Ninja" ..
ninja flash
```




