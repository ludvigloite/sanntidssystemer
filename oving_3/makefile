# program executable name
TARGET = oving3

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
	clang $(LDFLAGS) -fsanitize=address -o $@ $^

# compile
%.o : %.c
	clang -lpthread $(CFLAGS) -std=gnu11 -g -fsanitize=address -c -o $@ $<

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)