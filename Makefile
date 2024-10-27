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
clean:
	rm -rf frontend/static/js/*.js

# Install dependencies, compile TypeScript, and start server
setup: install-deps build-ts run-server
