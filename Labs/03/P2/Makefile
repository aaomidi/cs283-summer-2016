CC=gcc
FLAGS=-std=c++11 -w -lpthread
TARGETS=p2.o
build: 
	$(CC) main.c csapp.c $(FLAGS) -o $(TARGETS) 
clean:
	rm *.o
run: build
	./$(TARGETS) -p 1462
