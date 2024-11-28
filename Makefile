default: build

build:
	@echo "Building..."
	gcc main.cpp -o main -l pthread
	@echo "Build complete."
	@echo "Running..."
	./main

clean:
	@echo "Cleaning..."
	rm main
	@echo "Clean complete."