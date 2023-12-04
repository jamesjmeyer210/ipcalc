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

MAIN=src/main.c
SRC=src/lib

OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

TEST=test
TESTS=$(wildcard $(TEST)/*c)
TEST_BINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(TESTS))

LIB_DIR=lib
LIB = $(LIB_DIR)/libipcalc.a

$(LIB): $(LIB_DIR) $(OBJ) $(OBJS)
	$(RM) $(LIB)
	ar -rcs $(LIB) $(OBJS)

# Compile all the object files
$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile all the object files
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Collect all the *.c files in test/ and compile them to individual binaries, linking the library
$(TEST)/bin/%: $(TEST)/%.c
	$(CC) $(CFLAGS) $< -L./$(LIB_DIR) $(LIB) -o $@ -lcriterion

# Make the test binary directory
$(TEST)/bin:
	mkdir -p $@

# Make the obj directory
$(OBJ):
	mkdir -p $@

# Make the library directory
$(LIB_DIR):
	mkdir -p $@

# Removes the contents of all generated directories
clean:
	rm -v obj/*
	rm -v bin/*
	rm -v lib/*
	rm -v test/bin/*

all: $(LIB)

debug: all
	$(CC) $(CFLAGS) $(MAIN) -L./$(LIB_DIR) $(LIB) -o bin/ipcalc

test: $(LIB) $(TEST)/bin $(TEST_BINS)
	for test in $(TEST_BINS) ; do ./$$test ; done

# auto-generated
appstate.o:
	$(CC) $(CFLAGS) -c src/lib/appstate.c -o obj/appstate.o

bits.o:
	$(CC) $(CFLAGS) -c src/lib/bits.c -o obj/bits.o

convert.o:
	$(CC) $(CFLAGS) -c src/lib/convert.c -o obj/convert.o

error.o:
	$(CC) $(CFLAGS) -c src/lib/error.c -o obj/error.o

file.o:
	$(CC) $(CFLAGS) -c src/lib/file.c -o obj/file.o

ipv4addr.o:
	$(CC) $(CFLAGS) -c src/lib/ipv4addr.c -o obj/ipv4addr.o

ipv4regex.o:
	$(CC) $(CFLAGS) -c src/lib/ipv4regex.c -o obj/ipv4regex.o

list.o:
	$(CC) $(CFLAGS) -c src/lib/list.c -o obj/list.o

string.o:
	$(CC) $(CFLAGS) -c src/lib/string.c -o obj/string.o

main: appstate.o bits.o convert.o error.o file.o ipv4addr.o ipv4regex.o list.o string.o
	$(CC) $(CFLAGS) appstate.o bits.o convert.o error.o file.o ipv4addr.o ipv4regex.o list.o string.o  src/main.c -o bin/ipcalc
