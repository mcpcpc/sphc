.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
LDLIBS = -lm

all: main
main: main.o mat.o
	$(CC) $(ALL_LDFLAGS) -o main main.o mat.o $(LDLIBS)
main.o: src/main.c
	$(CC) $(ALL_CFLAGS) -c src/main.c
mat.o: src/mat.c src/mat.h
	$(CC) $(ALL_CFLAGS) -c src/mat.c src/mat.h
clean:
	rm -f main *.o
.PHONY: all clean