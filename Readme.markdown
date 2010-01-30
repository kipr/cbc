CBC Firmware
============
First off, remember that by following these instructions you may brick your CBC and void your warranty.  Also, this version of the code only works on the CBC 2.  You have been warned.

Requirements:
-------------
To build the CBC Firmware, You'll first need to have a Linux system up and running, preferrably Ubuntu 8.04.  You'll also need to install the following pieces of software by hand:

* ARM GCC Toolchain
* Qt Embedded 4.5

As well as the following minor pieces of software (easily installed through your distro's package manager)

* GCC
* GNU Make
* Qt 4.5
* Rsync
* Wget

### ARM GCC Toolchain ###
Instructions for downloading and installing the ARM GCC Toolchain necessary to build this software can found [here](http://wiki.chumby.com/mediawiki/index.php/GNU_Toolchain).  GCC 4.3.2 is required for this to work on your CBC.

### Qt Embedded ###
Instructions for building Qt Embedded can be found [here](http://www.jespersaur.com/drupal/node/30).  It is only necessary to follow the instructions until the installation of Qt is finished.  There's some useful info there though :)

Build:
---------

Building the software is simple:

    cd cbc
    make

You should now have a userhook0 file in filesystem/upgrade.  Happy Hacking!

