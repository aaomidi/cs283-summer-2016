CC=gcc
build: Assignment1.o
	echo "Building"

Assignment1.o: main.o hash.o
	$(CC) -o Assignment1.o main.o hash.o

hash.o: hash.c hash.h
	$(CC) -c hash.c

main.o: hash.h main.c
	$(CC) -c main.c

run: Assignment1.o
	./Assignment1.o

anagram: Assignment1.o
	./Assignment1.o -a $(WORD)

scrabble: Assignment1.o
	./Assignment1.o -s $(WORD) $(LETTER) $(PLACE)

clean:
	rm *.o