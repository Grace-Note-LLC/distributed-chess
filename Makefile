##
## Makefile for backend/frontend
##

# Variables
GO_CMD := go run main.go
NPM_CMD := npm install --prefix frontend
TSC := npx tsc -p tsconfig.json

# Install npm dependencies
install-deps:
	$(NPM_CMD)

# Compile TypeScript using tsconfig.json in static folder
build-ts:
	cd frontend && $(TSC)

# Run the Go server
run-server:
	cd server && $(GO_CMD) 

# Build TypeScript and run server
start: build-ts run-server

# Clean generated JavaScript files
server_clean:
	rm -rf frontend/static/js/*.js

# Install dependencies, compile TypeScript, and start server
setup: install-deps build-ts run-server

##
## Makefile for Game Engine
##

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Directories
SRC_DIR = dispatcher
BUILD_DIR = build
TEST_DIR = $(SRC_DIR)/test

# Source files
SOURCES = $(SRC_DIR)/Board.cpp $(SRC_DIR)/Move.cpp $(TEST_DIR)/test.cpp

# Object files in the build directory
OBJECTS = $(BUILD_DIR)/Board.o $(BUILD_DIR)/Move.o $(BUILD_DIR)/test.o

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile test executable
test: $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BUILD_DIR)/test -g

# Compile .cpp files to .o files in the build directory
$(BUILD_DIR)/Board.o: $(SRC_DIR)/Board.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Move.o: $(SRC_DIR)/Move.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test.o: $(TEST_DIR)/test.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
engine_clean:
	rm -rf $(BUILD_DIR)
