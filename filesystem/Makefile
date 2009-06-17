# Makefile for building the filesystems to go on the thumb drive
#

all: filesystems

make-kiss-ext2:
	make -C ext2

make-kiss-vfat:
	make -C vfat

kiss-ext2-install:
	rm -rf /mnt/kiss/*
	cp -a kiss-ext2/* /mnt/kiss

kiss-vfat-install:
	rm -rf /mnt/usb/*
	cp -r kiss-vfat/* /mnt/usb

filesystems: make-kiss-ext2 make-kiss-vfat

ifeq ($(NUKE),1)
install: kiss-ext2-install kiss-vfat-install
else
install: install-help
endif

install-help:
	@echo
	@echo 'WARNING: Running this installer will nuke anything in /mnt/usb'
	@echo '         and /mnt/kiss. Run "sudo make install NUKE=1" to proceed'
	@echo

clean:
	make -C ext2 clean
	make -C vfat clean
