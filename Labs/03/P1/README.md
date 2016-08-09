Lab 3 Part 1
-----

Amir Omidi
Systems Programming
Lab 3 Part 1
email: aao53@drexel.edu


This part of a lab consisted of writing a client to do HTTP get calls.

To do this I first made some CLI arguments. I wanted to experiment with making Linux-like arguments so I found out about "getopt". This allowed me to make arguments like: ./exec.o -p PORT_NUMBER -h HOST_NAME -f FILE_NAME

To compile this program use:
```
make build
```
To test how the program works:
```
make run
```

The "end of stream" detection is somewhat hacky, if mongoose library was used. A much nicer client could've been written.
