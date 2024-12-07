CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
MAIN = main
OBJS = main.o

all : $(MAIN)

$(MAIN) : $(OBJS) main.c
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm *.o $(MAIN) core