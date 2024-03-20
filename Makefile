CC=gcc
CFLAGS=-Iinclude -Wall
LDFLAGS=

SRCDIR=src
BINDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(SRCS:.c=.o)

TARGET=$(BINDIR)/githelp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lcurl

install:
	cp $(TARGET) /usr/local/bin

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean install
