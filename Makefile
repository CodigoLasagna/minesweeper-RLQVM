CXX := gcc -m32 -lncurses -lpanel
CFLAGS := -g -ansi -pedantic-errors
TARGET := main

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ 
clean:
	rm -rf $(TARGET) *.o
cleanO:
	rm -rf main.o
	
.PHONY: all clean
