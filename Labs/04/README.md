Lab 4
-----

----------

Amir Omidi

Systems Programming

Lab 4

email: aao53@drexel.edu

This lab was a very simple lab to help us learn to use threads and mutex locks in C/C++. 

The variables for this lab are:
/#define PEOPLE_COUNT 100 		<- Number of threads
/#define GETUP_COUNT 100000		<- Number of times they get up
/#define TRIALS 10							<- Number of trials

test1.txt includes the trace of running the program with no locks.
Test2 took on average 3.1 seconds to complete while Test1 were so fast that the system registered as 0 seconds.

## How to compile and use:

`make nolocks` Runs the program without using locks
`make locks` Runs the program with the lock inside the for loop
`make locksplus` Runs the program with the lock outside the for loop
`make clean` to clean created files
`make run` to run all of the trials.