SHELL=/bin/bash
run:
	python2 main.py $(FILENAME) $(NODES) 1
test:
	time python2 main.py $(FILENAME) 1 0
	time python2 main.py $(FILENAME) 2 0
	time python2 main.py $(FILENAME) 4 0
	time python2 main.py $(FILENAME) 8 0
	time python2 main.py $(FILENAME) 32 0 