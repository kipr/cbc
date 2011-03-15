userhook0: userhook0.body make-userhook0.pl filesystems
        ln -sf ../kiss-ext2/FIRMWARE_VERSION .
        ln -sf ../kiss-ext2/drivers/fb_print .
        ln -sf ../kiss-ext2/drivers/ext2.ko .
        ln -sf ../kiss-ext2/drivers/mkfs.ext2 .
        ln -sf ../kiss-ext2/drivers/rcS .
        ln -sf ../kiss-ext2/drivers/fdisk .

	tar -czf ext2.tgz -C ../kiss-ext2 .
	
        ./make-userhook0.pl fb_print ext2.ko ext2.tgz rcS mkfs.ext2 fdisk
	chmod a+x userhook0
        #mkdir -p tmp
        #cp -p userhook0 tmp/userhook0
        #tmp/userhook0 --extract

filesystems:
	make -C .. filesystems

clean:
        rm -rf ext2.tgz userhook0

distclean: clean
        rm -rf FIRMWARE_VERSION fb_print ext2.ko mkfs.ext2 rcS fdisk
