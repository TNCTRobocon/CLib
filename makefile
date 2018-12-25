#!/usr/bin/make
CC := gcc
TARGET := cmap.out
CFLAGS := -std=c89 -g -Wall -O0
FORMAT := clang-format
SRC:= test.c cmap.c
OBJ:= test.o cmap.o

.PHONY:all run debug clear format
all:$(TARGET)
$(TARGET):format $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
test.o:test.c
	$(CC) $(CFLAGS) -c $<
cmap.o:cmap.c
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