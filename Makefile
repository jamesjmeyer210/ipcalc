CC = clang
CFLAGS = -O \
	-Wextra \
	-Wall \
	-Wfloat-equal \
	-Wundef \
	-Wformat=2 \
	-g

clean:
	rm -v obj/*
	rm -v bin/*

convert.o:
	$(CC) -g -c $(CFLAGS) src/lib/convert.c -o obj/convert.o

bits.o:
	$(CC) -g -c $(CFLAGS) src/lib/bits.c -o obj/bits.o

list.o:
	$(CC) -g -c $(CFLAGS) src/lib/list.c -o obj/list.o

string.o: list.o
	$(CC) -g -c $(CFLAGS) src/lib/string.c -o obj/string.o

ipv4regex.o:
	$(CC) -g -c $(CFLAGS) src/ipv4regex.c -o obj/ipv4regex.o

ipv4addr.o: ipv4regex.o string.o
	$(CC) -g -c $(CFLAGS) src/ipv4addr.c -o obj/ipv4addr.o

appstate.o: list.o
	$(CC) -g -c $(CFLAGS) src/appstate.c -o obj/appstate.o

all: bits.o convert.o appstate.o ipv4regex.o ipv4addr.o
	$(CC) $(CFLAGS) src/main.c \
		obj/convert.o \
		obj/bits.o \
		obj/appstate.o \
		obj/ipv4regex.o \
		obj/ipv4addr.o \
  		obj/list.o \
  		obj/string.o \
		-o bin/ipcalc

test: all
	$(CC) $(CFLAGS) test/test.c \
	obj/convert.o \
	obj/bits.o \
 	obj/ipv4regex.o \
	obj/ipv4addr.o \
	obj/list.o \
	obj/string.o \
 	-o bin/ipcalc-test