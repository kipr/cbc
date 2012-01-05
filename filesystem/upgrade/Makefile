UPGRADEFILE = CBC_interface
OLDUPGRADEFILE = userhook0
cbc_interface: cbc_interface.body make-cbc_interface.pl filesystems
	ln -sf ../kiss-ext2/FIRMWARE_VERSION .
	ln -sf ../kiss-ext2/cbc_boot .
	ln -sf ../../utils/block_probe/block_probe .
	ln -sf ../../utils/fb_print/fb_print .

	tar -czf config.tgz -C ../kiss-ext2/ config
	tar -czf drivers.tgz -C ../kiss-ext2/ drivers
	tar -czf wifi.tgz -C ../kiss-ext2/ wifi
	tar -czf gui.tgz -C ../kiss-ext2/ gui
	tar -czf images.tgz -C ../kiss-ext2/ images
	tar -czf sounds.tgz -C ../kiss-ext2/ sounds
	tar -czf usercode.tgz -C ../kiss-ext2/ usercode
	tar -czf qt.tgz -C ../kiss-ext2/ qt 
	tar -czf gcc.tgz -C ../kiss-ext2/ gcc
	tar -czf jvm.tgz -C ../kiss-ext2/ jvm

	./make-cbc_interface.pl $(UPGRADEFILE) block_probe fb_print ext2.ko mkfs.ext2 mkdosfs ptable.bin cbc_boot config.tgz drivers.tgz wifi.tgz gui.tgz images.tgz sounds.tgz usercode.tgz qt.tgz gcc.tgz jvm.tgz

	./make-cbc_interface.pl $(OLDUPGRADEFILE) block_probe fb_print ext2.ko mkfs.ext2 mkdosfs ptable.bin cbc_boot config.tgz drivers.tgz wifi.tgz gui.tgz images.tgz sounds.tgz usercode.tgz qt.tgz gcc.tgz jvm.tgz

	 chmod a+x $(UPGRADEFILE)
	 chmod a+x $(OLDUPGRADEFILE)

filesystems:
	make -C ../ext2

clean:
	rm -rf $(UPGRADEFILE) FIRMWARE_VERSION cbc_boot block_probe fb_print *.tgz
	rm -rf $(OLDUPGRADEFILE) FIRMWARE_VERSION cbc_boot block_probe fb_print *.tgz 
