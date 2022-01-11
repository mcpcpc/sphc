.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
LDLIBS = -lm
PREFIX = /usr/local

all: clean main
main: main.o mat.o list.o sph.o
	$(CC) $(ALL_LDFLAGS) -o main main.o mat.o list.o sph.o $(LDLIBS)
main.o: main.c
sph.o: sph.c sph.h
mat.o: mat.c mat.h
list.o: list.c list.h
clean:
	rm -f main *.o

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: all clean