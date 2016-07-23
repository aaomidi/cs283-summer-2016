Lab 2
-----

Amir Omidi
Systems Programming
Lab 2
email: aao53@drexel.edu


In this lab we were to write a program that syncs two folders with each other according to the following rules:

* If a file in a does not exist in b, you should replicate it in b.
* If a file in b does not exist in a, it should be deleted from b.
* If a file exists in both a and b, the file with the most recent modified date / time should be copied from one directory to the other.

My program does all of this in a recursive manner, it does not care about file names and such. However the program works for FILES and DIRECTORIES only.

Clion with a Linux VM was used to develop and test the code for this assignment. The code, however, has been cloned to tux and tested there.

The program works perfectly according to the specs.
Sample runs:
```
Starting the sync...
Copying more recent file from B to A
Making file: /home/amir/Desktop/Dir1/randomFile3
Copying more recent file from B to A
Making file: /home/amir/Desktop/Dir1/me.jpg
Copying more recent file from A to B
Making file: /home/amir/Desktop/Dir2/otherFile
Started reverse traversal for deleting files.
Deleting directory from B: /home/amir/Desktop/Dir2/Blah/
	Finished.

```

To run and compile the code:
DIR1=/path/to/dir DIR2=/path/to/dir2 make sync

Its note worthy that according to valgrind, there are no memory leaks :)