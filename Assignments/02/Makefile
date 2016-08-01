CC=gcc
FLAGS=-I./src/ `pkg-config --cflags opencv` -fopenmp
TARGETS=hw3ex
LIBS=-lm `pkg-config --libs opencv` -fopenmp

main: $(TARGETS)


lib/%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) lib/*.o

%:lib/%.o lib/hw3.o
	$(CC) -o $@ $^ $(LIBS)
