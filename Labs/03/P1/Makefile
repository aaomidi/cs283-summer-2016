CC=gcc
FLAGS=-std=c++11 -w -lpthread
TARGETS=p1.o
build: 
	$(CC) main.c csapp.c $(FLAGS) -o $(TARGETS) 
clean:
	rm *.o
run: build
	./$(TARGETS) -h www.google.com -p 80 -f /index.html
