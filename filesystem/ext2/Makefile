CBC_GCC=CBC_GCC-4.3.2-Mini
CBC_GCC_TARFILE=$(CBC_GCC).tar.bz2

CBC_QT=CBC_Qt-4.7.1-Mini
CBC_QT_TARFILE=$(CBC_QT).tar.bz2

CBC_GUI_DIR=../../cbcui

TRACK_DIR=../../userlib/tracklib
LIBCBC_DIR=../../userlib/libcbc
INIT_DIR=../../userlib/init

SHARED_DIR=../../utils/shared_mem

KERNEL_DIR=../../kernel
CBOB_DIR=$(KERNEL_DIR)/cbob
MICRODIA_DIR=$(KERNEL_DIR)/microdia
RT73_DIR=$(KERNEL_DIR)/rt73-chumby-1.7.0/Module

DEST=$(CURDIR)/../kiss-ext2

all: filesystem

gcc: $(CBC_GCC_TARFILE)
	mkdir -p gcc
	tar xjf $(CBC_GCC_TARFILE) -C gcc
	touch gcc

qt: $(CBC_QT_TARFILE)
	mkdir -p qt
	tar xjf $(CBC_QT_TARFILE) -C qt
	touch qt

$(CBC_GCC_TARFILE):
	wget http://files.kipr.org/cbc/$(CBC_GCC_TARFILE)

$(CBC_QT_TARFILE):
	wget http://files.kipr.org/cbc/$(CBC_QT_TARFILE)

filesystem: gcc qt
	mkdir -p $(DEST)

	(cd files/gui; chmod a+x startup.sh shutdown.sh)
	(cd files/usercode; chmod a+x gcc-usercode run stop compile compile-usb)
	(cd files/wifi; chmod a+x wifi_scan.pl wifi_signal.pl wifi_connected.pl wifi_ip.pl wifi_start.pl)

	mkdir -p $(DEST)/usercode/include
	mkdir -p $(DEST)/usercode/lib
	mkdir -p $(DEST)/sounds
	mkdir -p $(DEST)/images
	mkdir -p $(DEST)/drivers
	mkdir -p $(DEST)/wifi
	mkdir -p $(DEST)/gui
	mkdir -p $(DEST)/config
	mkdir -p $(DEST)/gcc

	(cd files; rsync -a usercode/* $(DEST)/usercode)
	(cd files; rsync -a sounds/* $(DEST)/sounds)
	(cd files; rsync -a images/* $(DEST)/images)
	(cd files; rsync -a drivers/* $(DEST)/drivers)
	(cd files; rsync -a wifi/* $(DEST)/wifi)
	(cd files; rsync -a gui/* $(DEST)/gui)
	(cd files; rsync -a config/* $(DEST)/config)
	(cd gcc; rsync -a * $(DEST)/gcc)
	(cd qt; rsync -a * $(DEST))
#	(cd files; rsync -a gdb $(DEST)/bin)
	(cd files; rsync -a cbc_boot $(DEST))
	(cd files; rsync -a FIRMWARE_VERSION $(DEST))

	# kernel CBOB and camera modules
	make -C $(KERNEL_DIR)
	(cd $(CBOB_DIR); rsync -a cbob.ko $(DEST)/drivers)
	(cd $(MICRODIA_DIR); rsync -a microdia.ko $(DEST)/drivers)
	(cd $(RT73_DIR); rsync -a rt73.ko $(DEST)/drivers)

	make -C $(SHARED_DIR) DEST=$(DEST)/usercode/lib install_filesystem
	make -C $(TRACK_DIR) DEST=$(DEST) install_filesystem
	make -C $(INIT_DIR) DEST=$(DEST)/usercode/lib install_filesystem

	# CBC library
	make -C $(LIBCBC_DIR)
	(cd $(LIBCBC_DIR); rsync -a libcbc.a              	       $(DEST)/usercode/lib)
	(cd $(LIBCBC_DIR)/src; rsync -a cbcserial.h compat.h create.h $(DEST)/usercode/include)
	(cd $(LIBCBC_DIR)/src; rsync -a process.h botball.h cbc.h  $(DEST)/usercode/include)

	# CBC graphical interface
	make -C $(CBC_GUI_DIR)
	rsync -a $(CBC_GUI_DIR)/cbcui $(DEST)/gui

clean:
	rm -rf ../kiss-ext2

distclean: clean
	rm -rf gcc qt $(CBC_GCC_TARFILE) $(CBC_QT_TARFILE)

