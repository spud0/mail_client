CC = g++
LIBS = -lboost_system -lpthread -lssl -lcrypto

build: 
	$(CC) src/main.cc $(LIBS) -o main 
	bash -c 'set -a && . .env && set +a && ./main'  

clean: 
	rm -rf main
