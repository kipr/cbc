# Makefile for building the filesystems to go on the internal USB drive

all: filesystems

make-kiss-ext2:
	make -C ext2

kiss-ext2-install:
	rm -rf /mnt/kiss/*
	cp -a kiss-ext2/* /mnt/kiss

filesystems: make-kiss-ext2

ifeq ($(NUKE),1)
install: kiss-ext2-install
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
