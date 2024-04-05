CC=gcc
CFLAGS=-Iinclude -Wall
LDFLAGS=-lcurl -lm -lncurses

SRCDIR=src
INCDIR=src/include
BINDIR=usr/bin

#SRCS=$(wildcard $(SRCDIR)/*.c)
#OBJS=$(SRCS:.c=.o)
SRCS=$(wildcard $(SRCDIR)/*.c) $(wildcard $(INCDIR)/*.c)

TARGET=$(BINDIR)/githelp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

install:
	cp $(TARGET) /usr/local/bin

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(SRCDIR)/*.o
	rm -f $(INCDIR)/*.o
	rm -f $(BINDIR)/githelp
	rm -f /usr/local/bin/githelp

.PHONY: all clean install
