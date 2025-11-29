CC = g++

build: 
	$(CC) src/main.cc -o main
	./main

clean: 
	rm -rf main
