#!/usr/bin/make
CC := gcc
TARGET := cmap.out
CFLAGS := -std=c99 -g -Wall -O0
FORMAT := clang-format
SRC:= test.c ccontainer.c cpacket.c
OBJ:= test.o ccontainer.o cpacket.o

.PHONY:all run debug clear format
all:$(TARGET)
$(TARGET):clear format $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
test.o:test.c 
	$(CC) $(CFLAGS) -c $<
ccontainer.o:ccontainer.c
	$(CC) $(CFLAGS) -c $<
cpacket.o:cpacket.c
	$(CC) $(CFLAGS) -c $<
run:all
	./$(TARGET)
debug:all
	gdb ./$(TARGET)
clear:
	rm -f *.o
	rm -f $(TARGET)
format:
	$(FORMAT) -i $(SRC) $(INC)