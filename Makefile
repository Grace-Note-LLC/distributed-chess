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
CXXFLAGS = -std=c++11 -Wall -g -ftrapv
CXXTESTFLASGS = $(CXXFLAGS) -isystem /usr/local/include -pthread

# Directories
SRC_DIR = dispatcher
BUILD_DIR = build
TEST_DIR = $(SRC_DIR)/test

# Google Test library path
GTEST_LIB_PATH = /usr/local/lib

# Source files
SOURCES = $(SRC_DIR)/Board.cpp $(SRC_DIR)/Move.cpp $(TEST_DIR)/test.cpp
TEST_SOURCES = $(TEST_DIR)/test_king.cpp \
			   $(TEST_DIR)/test_bishop.cpp
# $(TEST_DIR)/test_queen.cpp $(TEST_DIR)/test_rook.cpp \
	 $(TEST_DIR)/test_knight.cpp $(TEST_DIR)/test_pawn.cpp


# Object files in the build directory
OBJECTS = $(BUILD_DIR)/Board.o $(BUILD_DIR)/Move.o $(BUILD_DIR)/test.o
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile test executable
test: $(BUILD_DIR) $(OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(TEST_OBJECTS) -L$(GTEST_LIB_PATH) -lgtest -lgtest_main -o $(BUILD_DIR)/test

# Compile .cpp files to .o files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXTESTFLAGS) -c $< -o $@

# Clean up build artifacts
engine_clean:
	rm -rf $(BUILD_DIR)
