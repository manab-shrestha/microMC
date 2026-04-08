CXX      := g++

HDF5_CFLAGS := $(shell pkg-config --cflags hdf5 2>/dev/null || echo "-I/opt/homebrew/include")
HDF5_LIBS   := $(shell pkg-config --libs hdf5 2>/dev/null || echo "-L/opt/homebrew/lib -lhdf5") -lhdf5_cpp

CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -DNDEBUG -march=native -ffast-math -I include $(HDF5_CFLAGS)

TARGET  := micromc
SRC_DIR := source
BLD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BLD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean compdb smoke

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(HDF5_LIBS)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BLD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BLD_DIR):
	mkdir -p $(BLD_DIR)

-include $(DEPS)

clean:
	rm -rf $(BLD_DIR) $(TARGET)

compdb:
	bear -- $(MAKE) clean all

smoke: $(TARGET)
	python3 smoke_test.py
