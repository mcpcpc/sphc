.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin

all: main
install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f main $(DESTDIR)$(BINDIR)
	chmod 755 $(DESTDIR)$(BINDIR)/main
main: main.o
	$(CC) $(ALL_LDFLAGS) -o main main.o $(LDLIBS)
main.o: src/main.c include/mat.h
	$(CC) $(ALL_CFLAGS) -c src/main.c
clean:
	rm -f main *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
.PHONY: all install uninstall clean