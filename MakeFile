CC = clang
CFLAGS = -Wall -Wextra -O2
TARGET = fileclip

all: $(TARGET)

$(TARGET): fileclip.c
	$(CC) $(CFLAGS) -o $(TARGET) fileclip.c

install: $(TARGET)
	install -m 0755 $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)

clean:
	rm -f $(TARGET)
