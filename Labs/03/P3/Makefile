CC=gcc
FLAGS=-std=c++11 -w -lpthread -ldl
TARGETS=p3.o
build: 
	$(CC) main.c csapp.c mongoose.c $(FLAGS) -o $(TARGETS)
clean:
	rm *.o
run: build
	./$(TARGETS) -p 1462
