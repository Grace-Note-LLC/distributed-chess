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
CXXFLAGS = -std=c++17 -Wall -g -ftrapv

# Directories
SRC_DIR = dispatcher
TEST_DIR = $(SRC_DIR)/test
BUILD_DIR = build

# Source files
COMMON_SOURCES = $(SRC_DIR)/Board.cpp $(SRC_DIR)/Move.cpp $(SRC_DIR)/Bot.cpp
TEST_SOURCES = $(TEST_DIR)/test.cpp \
               $(TEST_DIR)/test_bishop.cpp \
               $(TEST_DIR)/test_board.cpp \
               $(TEST_DIR)/test_bot.cpp \
               $(TEST_DIR)/test_king.cpp \
               $(TEST_DIR)/test_knight.cpp \
               $(TEST_DIR)/test_pawn.cpp \
               $(TEST_DIR)/test_queen.cpp \
               $(TEST_DIR)/test_rook.cpp
DISPATCHER_MAIN = $(SRC_DIR)/Dispatcher.cpp


# Object files
COMMON_OBJECTS = $(BUILD_DIR)/Board.o $(BUILD_DIR)/Move.o $(BUILD_DIR)/Bot.o
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SOURCES))
DISPATCHER_OBJECTS = $(COMMON_OBJECTS) $(BUILD_DIR)/Dispatcher.o

# Ensure the build directory exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile Test Executable
test: $(BUILD_DIR) $(COMMON_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(COMMON_OBJECTS) $(TEST_OBJECTS) -lgtest -lgtest_main -pthread -o $(BUILD_DIR)/test

# Compile Dispatcher Executable
dispatcher: $(BUILD_DIR) $(DISPATCHER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(DISPATCHER_OBJECTS) -o $(BUILD_DIR)/dispatcher
	@echo "Starting Dispatcher Server..."
	@./build/dispatcher

# Compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
engine_clean:
	rm -rf $(BUILD_DIR)
