# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 -Iinclude -Isrc/shared -pthread

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Source files (server: recursively find all .cpp in src/server)
SERVER_SRC := $(shell find $(SRC_DIR)/server -name '*.cpp') $(wildcard $(SRC_DIR)/shared/*.cpp)
CLIENT_SRC := $(wildcard $(SRC_DIR)/client/*.cpp) $(wildcard $(SRC_DIR)/shared/*.cpp)
INCLUDE_SRC := $(wildcard $(INC_DIR)/*.cpp)

# Object files
SERVER_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SERVER_SRC)) \
              $(patsubst $(INC_DIR)/%.cpp, $(BUILD_DIR)/include/%.o, $(INCLUDE_SRC))
CLIENT_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CLIENT_SRC)) \
              $(patsubst $(INC_DIR)/%.cpp, $(BUILD_DIR)/include/%.o, $(INCLUDE_SRC))

# Targets
SERVER_BIN := $(BIN_DIR)/server
CLIENT_BIN := $(BIN_DIR)/client

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server
$(SERVER_BIN): $(SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ -pthread

# Build client
$(CLIENT_BIN): $(CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ -pthread

# Compile src .cpp to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile include .cpp to .o
$(BUILD_DIR)/include/%.o: $(INC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean



