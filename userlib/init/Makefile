# Makefile for init.o

all: init.o

# If we're on linux, cross-compile for Chumby
GXX=arm-linux-g++
GCC=arm-linux-gcc
AR=arm-linux-ar
RANLIB=arm-linux-ranlib

init.o: init.c
	$(GCC) -Wall -c init.c

clean:
	rm -f init.o

install_filesystem: init.o
	rsync -a init.o $(DEST)

