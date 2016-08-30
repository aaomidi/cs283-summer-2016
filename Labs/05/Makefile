SHELL=/bin/bash
run:
	python2 main.py $(FILENAME) $(NODES) 1
test:
	(time python2 main.py $(FILENAME) 1 0) &>> output.txt
	(time python2 main.py $(FILENAME) 2 0) &>> output.txt
	(time python2 main.py $(FILENAME) 4 0) &>> output.txt
	(time python2 main.py $(FILENAME) 8 0) &>> output.txt
	(time python2 main.py $(FILENAME) 16 0) &>> output.txt
	(time python2 main.py $(FILENAME) 32 0)  &>> output.txt
clean: output.txt
	rm output.txt