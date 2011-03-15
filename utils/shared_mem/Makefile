CHUMBY_TARGET = $(shell ../../filesystem/chumby-target)

# If we're on linux, cross-compile for Chumby
OS=$(shell uname -o)
ifeq ($(OS),GNU/Linux)
GXX=arm-linux-g++
GCC=arm-linux-gcc
AR=arm-linux-ar
else
GXX=g++
GCC=gcc
AR=ar
endif

all: libshared.a TestServer TestClient test_server test_client

libshared.a: shared_mem.c
	$(GCC) -Wall -g -c shared_mem.c
	rm -f $@
	$(AR) -q $@ shared_mem.o

TestServer: TestServer.cpp SharedMem.h TestStruct.h
	$(GXX) -Wall -g TestServer.cpp -o TestServer

TestClient: TestClient.cpp SharedMem.h TestStruct.h
	$(GXX) -Wall -g TestClient.cpp -o TestClient

test_server: test_server.c libshared.a
	$(GCC) -Wall -g test_server.c libshared.a -o test_server

test_client: test_client.c libshared.a
	$(GCC) -Wall -g test_client.c libshared.a -o test_client

clean:
	rm -f *.o TestServer TestClient test_server test_client libshared.a

install: libshared.a
	scp libshared.a root@$(CHUMBY_TARGET):/mnt/kiss/usercode/lib

install_filesystem: libshared.a $(DEST)
	rsync -av libshared.a $(DEST)

