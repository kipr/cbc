userhook0: userhook0.body make-userhook0.pl filesystems
	ln -sf ../kiss-ext2/FIRMWARE_VERSION .
	ln -sf ../kiss-ext2/cbc_boot .
	ln -sf ../../utils/block_probe/block_probe .
	ln -sf ../../utils/fb_print/fb_print .

#	tar -czf ext2.tgz -C ../kiss-ext2 .

	tar -czf config.tgz -C ../kiss-ext2/ config
	tar -czf drivers.tgz -C ../kiss-ext2/ drivers
	tar -czf gui.tgz -C ../kiss-ext2/ gui
	tar -czf images.tgz -C ../kiss-ext2/ images
	tar -czf sounds.tgz -C ../kiss-ext2/ sounds
	tar -czf usercode.tgz -C ../kiss-ext2/ usercode
	tar -czf qt.tgz -C ../kiss-ext2/ qt 
	tar -czf gcc.tgz -C ../kiss-ext2/ gcc
	
	./make-userhook0.pl block_probe fb_print ext2.ko mkfs.ext2 mkdosfs ptable.bin cbc_boot config.tgz drivers.tgz gui.tgz images.tgz sounds.tgz usercode.tgz qt.tgz gcc.tgz
 
	chmod a+x userhook0

filesystems:
	make -C ../ext2

clean:
	rm -rf userhook0 FIRMWARE_VERSION cbc_boot block_probe fb_print *.tgz 
