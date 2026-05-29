CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I. -I../lab2
ASAN_FLAGS = -fsanitize=address
LDFLAGS = 

SRCS = main.cpp tests.cpp menu.cpp Matrix.cpp PolynomialUtils.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = lab3

.PHONY: all asan valgrind clean rebuild run debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) -c $< -o $@

asan: $(TARGET)
	./$(TARGET)

valgrind: clean
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

debug: $(TARGET)
	gdb ./$(TARGET)