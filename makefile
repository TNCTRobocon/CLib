#!/usr/bin/make
CC := gcc
TARGET := cmap.out
CFLAGS := -std=c99 -g -Wall -O0
FORMAT := clang-format
SRC:= test.c container.c network.c controllor.c
OBJ:= test.o container.o network.o controllor.o

.PHONY:all run debug clear format
all:$(TARGET)
$(TARGET):clear format $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
test.o:test.c 
	$(CC) $(CFLAGS) -c $<
container.o:container.c
	$(CC) $(CFLAGS) -c $<
network.o:network.c
	$(CC) $(CFLAGS) -c $<
controllor.o:controllor.c
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