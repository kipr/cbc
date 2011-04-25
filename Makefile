QTEMBEDDED_QMAKE=/mnt/kiss/qt/bin/qmake

all: cbc_interface

clean: cbc_interface_clean

fb_print:
	make -C utils/fb_print

fb_print_clean:
	make -C utils/fb_print clean

block_probe:
	make -C utils/block_probe

block_probe_clean:
	make -C utils/block_probe clean

shared_mem:
	make -C utils/shared_mem

shared_mem_clean:
	make -C utils/shared_mem clean

tracklib: shared_mem
	make -C userlib/tracklib

tracklib_clean:
	make -C userlib/tracklib clean

libcbc: shared_mem
	(cd userlib/libcbc && $(QTEMBEDDED_QMAKE))
	make -C userlib/libcbc

libcbc_clean:
	(if test -e userlib/libcbc/Makefile; then make -C userlib/libcbc clean; fi)

userlib: tracklib libcbc
	make -C userlib/init

userlib_clean: libcbc_clean tracklib_clean
	make -C userlib/init clean

cbcui: userlib
	(cd cbcui && $(QTEMBEDDED_QMAKE))
	make -s -C cbcui

cbcui_clean:
	(if test -e cbcui/Makefile; then make -C cbcui distclean; fi)

cbc_interface: cbcui userlib fb_print block_probe
	make -C filesystem/upgrade

cbc_interface_clean: cbcui_clean userlib_clean fb_print_clean block_probe_clean
	make -C filesystem/upgrade clean

.PHONY: cbc_interface cbcui userlib libcbc tracklib shared_mem fb_print block_probe
