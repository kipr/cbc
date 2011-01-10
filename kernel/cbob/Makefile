#KERNEL_DIR=../linux-2.6.16
KERNEL_DIR=../linux-2.6.16-chumby-1.7.0

obj-m += cbob.o

cbob-objs := cbob_main.o cbob_spi.o cbob_digital.o cbob_status.o
cbob-objs += cbob_analog.o cbob_pwm.o cbob_sensors.o 
cbob-objs += cbob_accel.o cbob_servo.o cbob_pid.o cbob_uart.o

all: build

clean:
	rm -rf *.o *.ko
	rm -rf *~
	rm -rf *.mod.c
	rm -rf .cbob*
	rm -rf .tmp_*
$(KERNEL_DIR):
	make -C .. kernel

build: $(KERNEL_DIR)
	make ARCH=arm BOARD=mx21ads CROSS_COMPILE=arm-linux- SUBDIRS=`pwd` -C $(KERNEL_DIR) modules
