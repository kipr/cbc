CBC_GCC=CBC_GCC-4.1.1-Mini
CBC_GCC_TARFILE=$(CBC_GCC).tar.bz2
SHARED_DIR=../../utils/shared_mem
FIRMWARE_DIR=../../cbcui
TRACK_DIR=../../userlib/tracklib
LIBCBC_DIR=../../userlib/libcbc
INIT_DIR=../../userlib/init

CBC_QT=CBC_Qt-4.4.3-Mini
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
	(cd gcc; rsync -a * $(DEST))
	(cd qt; rsync -a * $(DEST))
	#tar xvjf $(CBC_GCC_TARFILE) -C $(DEST)
	#tar xvjf $(CBC_QT_TARFILE) -C $(DEST)
	#rsync -a mnt/kiss/ $(DEST)

	mkdir -p $(DEST)/usercode
	mkdir -p $(DEST)/usercode/include
	mkdir -p $(DEST)/usercode/lib

	(cd files; chmod a+x startup.sh shutdown.sh run_qt_app.sh gcc-usercode run stop compile compile-usb)
	(cd files; rsync -a startup.sh run_qt_app.sh shutdown.sh $(DEST))
	(cd files; rsync -a gcc-usercode run stop compile compile-usb $(DEST)/usercode)
	(cd files; rsync -a gdb                                  $(DEST)/bin)
	(cd files; rsync -a ts.conf                              $(DEST)/qt/etc)
	(cd files; rsync -a beep.wav														 $(DEST))

	# First make sure libcbc is updated
	make -C $(LIBCBC_DIR)
  # Now copy
	(cd $(LIBCBC_DIR); rsync -a libcbc.a              	       $(DEST)/usercode/lib)
	(cd $(LIBCBC_DIR)/src; rsync -a cbcserial.h compat.h create.h $(DEST)/usercode/include)
	(cd $(LIBCBC_DIR)/src; rsync -a process.h botball.h cbc.h  $(DEST)/usercode/include)

	make -C $(SHARED_DIR) DEST=$(DEST) install_filesystem
	make -C $(TRACK_DIR) DEST=$(DEST) install_filesystem
	make -C $(INIT_DIR) DEST=$(DEST) install_filesystem
	make -C $(FIRMWARE_DIR)
	rsync -a $(FIRMWARE_DIR)/cbcui $(DEST)

clean:
	rm -rf ../kiss-ext2

distclean:
	rm -rf gcc qt $(CBC_GCC_TARFILE) $(CBC_QT_TARFILE)

