Lab 5
-------------------

Amir Omidi

Systems Programming

Lab 5

email: aao53@drexel.edu


----------
### Introduction:
In this assignment we were tasked with creating a mapreduce program. This map reduce program analyzed the dataset of Septa's trains which was published on Kaggle. This program found out what percent of the trains are on time for each specific route.

This analysis can be further extended to finding a perfectly efficient routing and time table for Septa.


--------

### Decoding the Results:
As we see in the results.pdf file, running the program with 8 nodes was the most optimal.

Lets analyze why this is the case, we ran the code on Drexel's tux machine. The output from `lscpu` on the machine is: 
    
    Architecture:          x86_64
    CPU op-mode(s):        32-bit, 64-bit
    Byte Order:            Little Endian
    CPU(s):                32
    On-line CPU(s) list:   0-31
    Thread(s) per core:    1
    Core(s) per socket:    8
    Socket(s):             4
    NUMA node(s):          8
    Vendor ID:             AuthenticAMD
    CPU family:            16
    Model:                 9
    Stepping:              1
    CPU MHz:               2400.000
    BogoMIPS:              4800.15
    Virtualization:        AMD-V
    L1d cache:             64K
    L1i cache:             64K
    L2 cache:              512K
    L3 cache:              5118K
    NUMA node0 CPU(s):     0-3
    NUMA node1 CPU(s):     4-7
    NUMA node2 CPU(s):     8-11
    NUMA node3 CPU(s):     12-15
    NUMA node4 CPU(s):     16-19
    NUMA node5 CPU(s):     20-23
    NUMA node6 CPU(s):     24-27
    NUMA node7 CPU(s):     28-31

As you can see, `CPU(s):` shows the number of threads available for us to use. Even though we have 32 threads, we were slowed down while using 16 and 32 threads.

There are multiple causes for this:

1. I wasn't the only person using the Tux machine while running this. Not all threads were empty.
2. Creating threads is expensive, at some point the data size just isn't big enough to benefit from making more threads.

So we can reach the conclusion that more threads doesn't always necessarily mean a good thing. We should adjust the number of nodes and threads our program uses by considering the size of the dataset.

------------

### How to run:

To run the program with a specific node (thread) count, we can use the following command. We also need to specify our data set for the program.
`FILENAME=otp.csv NODES=8 make run`

To run a test of the program with 1, 2, 4, 8, 16, and 32 nodes we do the following:
`FILENAME=otp.csv make test`
This will create a file called output.txt with the runtime of all tests. The `real` time is the actual time the program took to complete its goal.


I would like to make a special thanks to the Kaggle team for providing an amazing service. This program would not have been possible with them and Septa.
