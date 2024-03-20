CC=gcc
CFLAGS=-Iinclude -Wall
LDFLAGS=

SRCDIR=src
BINDIR=bin

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(SRCS:.c=.o)

TARGET=$(BINDIR)/your_project

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(LDFLAGS) $^ -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

install:
    cp $(TARGET) /usr/local/bin

clean:
    rm -f $(OBJS) $(TARGET)

.PHONY: all clean install
