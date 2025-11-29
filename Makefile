CC = g++
LIBS = -lboost_system -lpthread -lssl -lcrypto

build: 
	$(CC) src/main.cc $(LIBS) -o main 
	./main

clean: 
	rm -rf main
