CC=gcc -g

build: Lab2.o
	echo "Building"
Lab2.o: main.c
	$(CC) main.c -o Lab2.o
sync: build
	./Lab2.o $(DIR1) $(DIR2)
clean:
	rm *.o