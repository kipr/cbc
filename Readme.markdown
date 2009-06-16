CBC Firmware
============
First off, remember that by following these instructions you may brick your CBC and void your warranty.  You have been warned.

Requirements:
-------------
To build the CBC Firmware, You'll first need to have a Linux system up and running, preferrably Ubuntu 8.04.  You'll also need to install the following pieces of software by hand:

* ARM GCC Toolchain
* Qt Embedded 4.4

As well as the following minor pieces of software (easily installed through your distro's package manager)

* GCC
* GNU Make
* Qt 4.4
* Rsync
* Wget

### ARM GCC Toolchain ###
Instructions for downloading and installing the ARM GCC Toolchain necessary to build this software can found [here](http://wiki.chumby.com/mediawiki/index.php/GNU_Toolchain).  Currently only the 4.1.2b toolchain is known to work.

### Qt Embedded ###
Instructions for building Qt Embedded can be found [here](http://www.jespersaur.com/drupal/node/30).  It is only necessary to follow the instructions until the installation of Qt is finished.  There's some useful info there though :)

Build:
---------
Someday this will be a script, but for now build things in this order:


1.  utils
        cd utils/fb_print
        make
        cd utils/shared_mem
        make
2.  userlib
        cd userlib/init
        make
        cd ../libcbc
        qtembedded-qmake
        make
        cd ../tracklib
        make
3.  cbcui
        cd cbcui
        qtembedded-qmake
        make
4.  Filesystem
        cd filesystem/upgrade
        make

You should now have a userhook0 file in filesystem/upgrade.  Happy Hacking!

