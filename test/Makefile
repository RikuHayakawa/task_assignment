# Compiler and flags
CXX = g++

# Target and source files
BUILD_DIR = build
TARGET = generator
SRCS = generator.cpp

# Default target
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the executable
$(BUILD_DIR)/$(TARGET): $(SRCS)
	$(CXX) -o $(BUILD_DIR)/$(TARGET) $(SRCS)

# Run the program
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# Clean the build artifacts
clean:
	rm -rf $(BUILD_DIR)
