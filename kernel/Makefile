COMPILER = /usr/bin/arm-linux-gcc

TARFILE = linux-2.6.16-chumby-1.7.0.tar.gz
KERNEL_DIR = linux-2.6.16-chumby-1.6.0
KERNEL = $(KERNEL_DIR)/arch/arm/boot/zImage
ZIPFILE = k1.bin.zip
DOWNLOAD_ADDR = http://files.kipr.org/cbc/$(TARFILE)

CBOB_DIR = cbob
CBOB_MODULE = $(CBOB_DIR)/cbob.ko

MICRODIA_DIR = microdia
MICRODIA_MODULE = $(MICRODIA_DIR)/microdia.ko

all: $(ZIPFILE) $(MICRODIA_MODULE) $(CBOB_MODULE)

kernel: $(KERNEL)
cbc_spi: $(CBOB_MODULE)
microdia: $(MICRODIA_MODULE)

$(ZIPFILE): $(KERNEL)
	perl align.pl $(KERNEL); zip k1.bin.zip $(KERNEL)

$(MICRODIA_MODULE): $(KERNEL)
	make -C $(MICRODIA_DIR) -f CBCMakefile

$(CBOB_MODULE): $(KERNEL)
	make -C $(CBOB_DIR)

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

