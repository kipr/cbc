userhook0: userhook0.body make-userhook0.pl filesystems
	ln -sf ../kiss-ext2/FIRMWARE_VERSION .
	ln -sf ../kiss-ext2/drivers/fb_print .
	ln -sf ../../utils/block_probe/block_probe .

	tar -czf ext2.tgz -C ../kiss-ext2 .

	./make-userhook0.pl fb_print ext2.ko ext2.tgz mkfs.ext2 mkdosfs ptable.bin block_probe
	chmod a+x userhook0
        #mkdir -p tmp
        #cp -p userhook0 tmp/userhook0
        #tmp/userhook0 --extract

filesystems:
	make -C ../ext2

clean:
	rm -rf ext2.tgz userhook0 FIRMWARE_VERSION block_probe fb_print 
