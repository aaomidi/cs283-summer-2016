CC=gcc
FLAGS=-std=c++11 -w -lpthread
TARGETS=04.o
build: 
	$(CC) main.c csapp.c $(FLAGS) -o $(TARGETS) 
clean:
	rm *.o
all: build
	./$(TARGETS)
nolocks: build
	./$(TARGETS) 1
locks: build
	./$(TARGETS) 2
locksplus: build
	./$(TARGETS) 3