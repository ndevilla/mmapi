
CFLAGS = -O2 -g -I. -Wall -pedantic

all: mmapi.o

example: mmapi

mmapi: mmapi.c mmapi.h
	$(CC) $(CFLAGS) -DMAIN -o $@ $<

clean:
	rm -f mmapi *.o
