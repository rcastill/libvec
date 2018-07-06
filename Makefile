CC=clang
OUT=out
SRC=src
PREFIX=/usr/local

all: libvec

init:
	mkdir -p $(OUT)

libvec: init
	$(CC) -c $(SRC)/vec.c -o $(OUT)/libvec.o
	ar rcs $(OUT)/libvec.a $(OUT)/libvec.o

install: libvec
	cp $(OUT)/libvec.a $(PREFIX)/lib
	cp $(SRC)/vec.h $(PREFIX)/include

uninstall:
	rm -f $(PREFIX)/lib/libvec.a
	rm -f $(PREFIX)/include/vec.h

clean:
	rm -f $(OUT)/*.o

nuke:
	rm -rf $(OUT)
