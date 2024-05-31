# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -g -Wall -std=c99
CXXFLAGS = -g -Wall -std=c++14

# Directories
INCLUDE_DIR = include
SRC_DIR = src

# Source files
C_SRCS = $(SRC_DIR)/Image.c $(SRC_DIR)/Line.c
CXX_SRCS = $(SRC_DIR)/anti-alias.cpp $(SRC_DIR)/mytest.cpp $(SRC_DIR)/ball.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CXX_OBJS = $(CXX_SRCS:.cpp=.o)

# Target executable
TARGET = assignment3

# Build rules
all: $(TARGET)

$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

clean:
	rm -f $(C_OBJS) $(CXX_OBJS) $(TARGET)

.PHONY: all clean
