CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -pedantic -std=c++23 -O3 -march=native

TARGET := wer-calculator

SRC := src/main.cpp src/helper.cpp
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean