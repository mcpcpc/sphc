.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
LDLIBS = -lm

all: clean main
main: main.o mat.o list.o
	$(CC) $(ALL_LDFLAGS) -o main main.o mat.o list.o $(LDLIBS)
main.o: src/main.c
	$(CC) $(ALL_CFLAGS) -c src/main.c
sph.o: src/sph.c src/sph.h
	$(CC) $(ALL_CFLAGS) -c src/sph.c src/sph.h
mat.o: src/mat.c src/mat.h
	$(CC) $(ALL_CFLAGS) -c src/mat.c src/mat.h
list.o: src/list.c src/list.h
	$(CC) $(ALL_CFLAGS) -c src/list.c src/list.h
clean:
	rm -f main *.o
.PHONY: all clean