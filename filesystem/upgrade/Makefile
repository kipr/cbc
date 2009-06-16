userhook0: userhook0.body make-userhook0.pl filesystems
	ln -sf ../kiss-vfat/FIRMWARE_VERSION .
	ln -sf ../kiss-vfat/fb_print .
	ln -sf ../kiss-vfat/ext2.ko .
	ln -sf ../kiss-vfat/mkfs.ext2 .
	ln -sf ../kiss-vfat/rcS .
	
	ln -sf files/block_probe .
	ln -sf files/mkdosfs .
	ln -sf files/ptable.bin .
	
	tar -czf vfat.tgz -C ../kiss-vfat .
	tar -czf ext2.tgz -C ../kiss-ext2 .
	
	./make-userhook0.pl fb_print ext2.ko vfat.tgz ext2.tgz rcS block_probe mkdosfs ptable.bin mkfs.ext2
	chmod a+x userhook0
	mkdir -p tmp
	cp -p userhook0 tmp/userhook0
	tmp/userhook0 --extract

foo:

	ls -l userhook0
	tar cvfh - FIRMWARE_VERSION fb_print ext2.ko vfat.tgz ext2.tgz > tmp.tar
	cat tmp.tar >> userhook0

filesystems:
	make -C .. filesystems

