CC=gcc
CFLAGS=-Iinclude -Wall
LDFLAGS=-lcurl -lm -lncurses

SRCDIR=src
INCDIR=src/include
BINDIR=bin

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

.PHONY: all clean install
