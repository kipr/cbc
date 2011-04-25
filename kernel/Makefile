COMPILER = /usr/bin/arm-linux-gcc

# update for new chipset July 27, 2010 kernel
#TARFILE = linux-2.6.16.tgz
TARFILE = linux-2.6.16-chumby-1.7.0.tar.gz
# update for new chipset july 27, 2010 kernel
#KERNEL_DIR = linux-2.6.16
KERNEL_DIR = linux-2.6.16-chumby-1.7.0
KERNEL = $(KERNEL_DIR)/arch/arm/boot/zImage
ZIPFILE = k1.bin.zip
DOWNLOAD_ADDR = http://files.kipr.org/cbc/$(TARFILE)

CBOB_DIR = cbob
CBOB_MODULE = $(CBOB_DIR)/cbob.ko

MICRODIA_DIR = microdia
MICRODIA_MODULE = $(MICRODIA_DIR)/microdia.ko

RT73_DIR = rt73-chumby-1.7.0
RT73_MODULE = $(RT73_DIR)/Module/rt73.ko
RT73_TARFILE = $(RT73_DIR).tar.gz
RT73_DOWNLOAD = http://files.kipr.org/cbc/$(RT73_TARFILE)

all: $(ZIPFILE) $(MICRODIA_MODULE) $(CBOB_MODULE) $(RT73_MODULE) 

kernel: $(KERNEL)
cbc_spi: $(CBOB_MODULE)
microdia: $(MICRODIA_MODULE)
rt73: $(RT73_MODULE)

$(ZIPFILE): $(KERNEL)
	perl align.pl $(KERNEL); zip k1.bin.zip $(KERNEL)

$(MICRODIA_MODULE): $(KERNEL)
	make -C $(MICRODIA_DIR) -f CBCMakefile

$(CBOB_MODULE): $(KERNEL)
	make -C $(CBOB_DIR)

$(RT73_MODULE): $(RT73_TARFILE) $(KERNEL)
	tar xvfz $(RT73_TARFILE)
	(cd $(RT73_DIR)/Module; make ARCH=arm CROSS_COMPILE=arm-linux- KERNDIR=../../$(KERNEL_DIR) arm)

$(RT73_TARFILE):
	wget $(RT73_DOWNLOAD)

$(KERNEL): $(KERNEL_DIR) $(COMPILER)
	ARCH=arm BOARD=mx21ads CROSS_COMPILE=arm-linux- make -C $(KERNEL_DIR)

$(KERNEL_DIR): $(TARFILE)
	tar xvfz $(TARFILE)
	touch $(KERNEL_DIR)
	rm -rf $(KERNEL_DIR)/scripts/setlocalversion
	echo "#!/bin/sh" > $(KERNEL_DIR)/scripts/setlocalversion

$(TARFILE):
ifneq ($(wildcard ../../files/$(TARFILE)), ../../files/$(TARFILE))
	wget $(DOWNLOAD_ADDR)
else
	ln -s ../../files/$(TARFILE) .
endif

$(COMPILER):
	@echo "Please install arm-linux-gcc toolchain (see http://wiki.chumby.com/mediawiki/index.php/GNU_Toolchain)"
	exit 1

distclean: clean
	rm -f $(TARFILE)
	rm -f $(ZIPFILE)
	rm -rf $(KERNEL_DIR)

clean:
	make -C $(MICRODIA_DIR) -f CBCMakefile clean	
	ARCH=arm BOARD=mx21ads CROSS_COMPILE=arm-linux- make -C $(KERNEL_DIR) clean

install_filesystem: $(ZIPFILE) $(MICRODIA_MODULE)
	mkdir -p $(DEST)/update2
	rsync -a $(ZIPFILE) $(DEST)/update2
	rsync -a $(MICRODIA_MODULE) $(DEST)

