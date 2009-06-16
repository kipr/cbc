KERNEL_DIR=../linux-2.6.16-chumby-1.6.0


cbc-objs := cbc_spi.o

obj-m += cbc_spi.o

all: build

clean:
	rm -rf *.o *.ko
	rm -rf *~
	rm -rf *.mod.c
	rm -rf .cbc_*
	rm -rf .tmp_*
$(KERNEL_DIR):
	make -C .. kernel

build: $(KERNEL_DIR)
	make ARCH=arm BOARD=mx21ads CROSS_COMPILE=arm-linux- SUBDIRS=`pwd` -C $(KERNEL_DIR) modules
