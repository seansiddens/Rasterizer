EXECBIN  = rasterizer

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror
LFLAGS   = -lm -lpthread

.PHONY: all clean

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(OBJECTS)

format:
	clang-format -i *.[ch] -style="{IndentWidth: 4, ColumnLimit: 90}"

