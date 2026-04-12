CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fsanitize=address -I. -I../lab2
LDFLAGS = -fsanitize=address

SRCS = main.cpp tests.cpp menu.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = lab3

.PHONY: all clean rebuild run valgrind test debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) --test

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)