KERNEL_DIR = ../../kernel
FB_PRINT_DIR = ../../utils/fb_print
DEST = $(CURDIR)/../kiss-vfat

filesystem:
	mkdir -p $(DEST)
	(cd files; rsync -a userhook0 ext2.ko cbob.ko microdia.ko FIRMWARE_VERSION videodev.ko $(DEST))
	(cd files; rsync -a rcS mkfs.ext2 fsck.ext2 $(DEST))
	mkdir -p $(DEST)/update2
	(cd files; rsync -a k1.bin.zip $(DEST)/update2)
	make -C $(FB_PRINT_DIR) DEST=$(DEST) install_filesystem

clean:
	rm -rf $(DEST)

install_userhook: filesystem
	scp $(DEST)/userhook0 $(DEST)/fb_print $(DEST)/FIRMWARE_VERSION root@`../chumby-target`:/mnt/usb
