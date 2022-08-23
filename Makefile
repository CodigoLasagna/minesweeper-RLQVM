CXX := gcc -m32
CFLAGS := -g
LDLIBS := -lncurses -lpanel
INC := ./res/
TARGET := main

SRCS := $(wildcard *.c $(INC)*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(LDLIBS) $^ -o $@ 
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(TARGET) *.o $(INC)*.o
	
.PHONY: all clean
