# program executable name
TARGET = target

# compiler flags
CFLAGS = -g -Wall

# linker flags
LDFLAGS = -g

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -fsanitize=address -o $@ $^

# compile
%.o : %.c
	gcc $(CFLAGS) -std=gnu11 -g -fsanitize=address -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)