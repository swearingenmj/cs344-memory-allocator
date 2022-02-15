CC=gcc
CCOPTS=-Wall -Wextra -Wno-deprecated-declarations -Wunused-but-set-variable
LIBS=

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

%: %.c
	$(CC) $(CCOPTS) -o $@ $< $(LIBS)