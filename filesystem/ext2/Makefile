CBC_GCC=CBC_GCC-4.3.2-Mini
CBC_GCC_TARFILE=$(CBC_GCC).tar.bz2
SHARED_DIR=../../utils/shared_mem
CBC_GUI_DIR=../../cbcui
TRACK_DIR=../../userlib/tracklib
LIBCBC_DIR=../../userlib/libcbc
INIT_DIR=../../userlib/init

CBC_QT=CBC_Qt-4.7.1-Mini
CBC_QT_TARFILE=$(CBC_QT).tar.bz2

WGET=wget
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
	$(WGET) http://files.kipr.org/cbc/$(CBC_GCC_TARFILE)

$(CBC_QT_TARFILE):
	$(WGET) http://files.kipr.org/cbc/$(CBC_QT_TARFILE)

filesystem: gcc qt
	mkdir -p $(DEST)

        (cd files/gui; chmod a+x startup.sh shutdown.sh)
        (cd files/usercode; chmod a+x gcc-usercode run stop compile compile-usb)

        mkdir -p $(DEST)/usercode/include
        mkdir -p $(DEST)/usercode/lib
        mkdir -p $(DEST)/sounds
        mkdir -p $(DEST)/images
        mkdir -p $(DEST)/drivers
        mkdir -p $(DEST)/gui
        mkdir -p $(DEST)/config

        (cd files; rsync -a usercode/* $(DEST)/usercode)
        (cd files; rsync -a sounds/* $(DEST)/sounds)
        (cd files; rsync -a images/* $(DEST)/images)
        (cd files; rsync -a drivers/* $(DEST)/drivers)
        (cd files; rsync -a gui/* $(DEST)/gui)
	(cd gcc; rsync -a * $(DEST))
	(cd qt; rsync -a * $(DEST))
        (cd files; rsync -a ts_vars.env $(DEST))
        (cd files; rsync -a gdb $(DEST)/bin)
        (cd files; rsync -a ts.conf $(DEST)/qt/etc)

	# First make sure libcbc is updated
	make -C $(LIBCBC_DIR)
	(cd $(LIBCBC_DIR); rsync -a libcbc.a              	       $(DEST)/usercode/lib)
	(cd $(LIBCBC_DIR)/src; rsync -a cbcserial.h compat.h create.h $(DEST)/usercode/include)
	(cd $(LIBCBC_DIR)/src; rsync -a process.h botball.h cbc.h  $(DEST)/usercode/include)

	make -C $(SHARED_DIR) DEST=$(DEST) install_filesystem
	make -C $(TRACK_DIR) DEST=$(DEST) install_filesystem
	make -C $(INIT_DIR) DEST=$(DEST) install_filesystem
        make -C $(CBC_GUI_DIR)
        rsync -a $(CBC_GUI_DIR)/cbcui $(DEST)/gui

clean:
	rm -rf ../kiss-ext2

distclean:
	rm -rf gcc qt $(CBC_GCC_TARFILE) $(CBC_QT_TARFILE)

