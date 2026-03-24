CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I include

TARGET  := micromc
SRC_DIR := source
BLD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BLD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BLD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BLD_DIR):
	mkdir -p $(BLD_DIR)

-include $(DEPS)

clean:
	rm -rf $(BLD_DIR) $(TARGET)
