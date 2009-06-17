QTEMBEDDED_QMAKE=/mnt/kiss/qt/bin/qmake

all: userhook0

clean: userhook0_clean

fb_print:
	make -C utils/fb_print

fb_print_clean:
	make -C utils/fb_print clean

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
	make -C cbcui

cbcui_clean:
	(if test -e cbcui/Makefile; then make -C cbcui distclean; fi)

userhook0: cbcui userlib fb_print
	make -C filesystem/upgrade
	ln -sf filesystem/upgrade/userhook0 userhook0

userhook0_clean: cbcui_clean userlib_clean fb_print_clean
	make -C filesystem/upgrade clean
	rm -f userhook0

.PHONY: userhook0 cbcui userlib libcbc tracklib shared_mem fb_print
