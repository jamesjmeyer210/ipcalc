CC = clang
CFLAGS = -O \
	-Wextra \
	-Wall \
	-Wfloat-equal \
	-Wundef \
	-Wformat=2 \
	-g

OBJ_FILES = obj/error.o \
	obj/convert.o \
	obj/bits.o \
	obj/appstate.o \
	obj/ipv4regex.o \
	obj/ipv4addr.o \
	obj/array.o \
	obj/list.o \
	obj/string.o \
	obj/file.o

clean:
	rm -v obj/*
	rm -v bin/*

error.o:
	$(CC) -g -c $(CFLAGS) src/lib/error.c -o obj/error.o

convert.o: error.o
	$(CC) -g -c $(CFLAGS) src/lib/convert.c -o obj/convert.o

bits.o:
	$(CC) -g -c $(CFLAGS) src/lib/bits.c -o obj/bits.o

bits-test:
	$(CC) $(CFLAGS) -D TEST=1 src/lib/bits.c -o bin/bits-test

list.o:
	$(CC) -g -c $(CFLAGS) src/lib/list.c -o obj/list.o

string.o: list.o
	$(CC) -g -c $(CFLAGS) src/lib/string.c -o obj/string.o

file.o: list.o string.o
	$(CC) -g -c $(CFLAGS) src/lib/file.c -o obj/file.o

ipv4regex.o:
	$(CC) -g -c $(CFLAGS) src/ipv4regex.c -o obj/ipv4regex.o

ipv4addr.o: error.o convert.o ipv4regex.o string.o
	$(CC) -g -c $(CFLAGS) src/ipv4addr.c -o obj/ipv4addr.o

appstate.o: list.o
	$(CC) -g -c $(CFLAGS) src/appstate.c -o obj/appstate.o

lib: file.o error.o bits.o convert.o appstate.o ipv4regex.o ipv4addr.o
	ar -rcs lib/libipcalc.a obj/*.o

all: lib
	$(CC) -o bin/ipcalc src/main.c lib/libipcalc.a

test: all
	$(CC) $(CFLAGS) test/test.c $(OBJ_FILES) -o bin/ipcalc-test