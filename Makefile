CC = gcc
CFLAGS = -Wall
EXEC = main

OBJS = main.o huffman.o minheap.o utils.o

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

main.o: main.c huffman.h minheap.h
	$(CC) $(CFLAGS) -c main.c

huffman.o: huffman.c huffman.h minheap.h
	$(CC) $(CFLAGS) -c huffman.c

minheap.o: minheap.c minheap.h huffman.h
	$(CC) $(CFLAGS) -c minheap.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c