SHARED_MEM = ../../utils/shared_mem
CHUMBY_TARGET = $(shell ../../filesystem/chumby-target)
all: libtrack.a sample_tracklib_client

# If we're on linux, cross-compile for Chumby
OS=$(shell uname -o)
ifeq ($(OS),GNU/Linux)
GXX=arm-linux-g++
GCC=arm-linux-gcc
AR=arm-linux-ar
RANLIB=arm-linux-ranlib
else
GXX=g++
GCC=gcc
AR=ar
RANLIB=ranlib
endif

INCLUDES = -I../../cbcui/src/vision -I$(SHARED_MEM)

tracklib.o: tracklib.c track.h 
	$(GCC) -Wall -c $(INCLUDES) tracklib.c

shared_mem.o: $(SHARED_MEM)/shared_mem.c
	$(GCC) -Wall -c $(INCLUDES) $^

libtrack.a: tracklib.o
	rm -f $@
	$(AR) -q $@ $^
	$(RANLIB) $@

#sample_tracklib_client: sample_tracklib_client.c track.h tracklib.o shared_mem.o
#	$(GCC) -Wall sample_tracklib_client.c -o sample_tracklib_client tracklib.o shared_mem.o

sample_tracklib_client: sample_tracklib_client.c track.h libtrack.a shared_mem.o
	$(GCC) -Wall sample_tracklib_client.c shared_mem.o -o sample_tracklib_client libtrack.a 

clean:
	rm -f *.o *.a sample_tracklib_client

install: libtrack.a track.h
	scp libtrack.a root@$(CHUMBY_TARGET):/mnt/kiss/usercode/lib
	scp track.h root@$(CHUMBY_TARGET):/mnt/kiss/usercode/include

install_filesystem: libtrack.a track.h
	rsync -a libtrack.a $(DEST)/usercode/lib
	rsync -a track.h $(DEST)/usercode/include

