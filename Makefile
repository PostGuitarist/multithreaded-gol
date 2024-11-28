default: build

build:
	@echo "Building..."
	gcc connell_pthread.cpp -o connell_pthread -l pthread
	@echo "Build complete."
	@echo "Running..."
	./connell_pthread

clean:
	@echo "Cleaning..."
	rm connell_pthread
	@echo "Clean complete."