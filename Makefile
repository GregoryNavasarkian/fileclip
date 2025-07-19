CC = clang
CFLAGS = -Wall -Wextra -pedantic -O2 -std=c99 -D_FORTIFY_SOURCE=2
TARGET = fileclip
.PHONY: all install uninstall clean test

PREFIX ?= /usr/local/bin

all: $(TARGET)

$(TARGET): fileclip.c
	$(CC) $(CFLAGS) -o $(TARGET) fileclip.c

install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)/$(TARGET)

uninstall:
	rm -f $(PREFIX)/$(TARGET)

clean:
	rm -f $(TARGET)

test:
	@echo "No tests defined. Add tests to validate the build."

help:
	@echo "Targets: all, install, uninstall, clean, test"
