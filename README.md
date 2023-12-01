# Deadlock avoidance
# Banker's Algorithm
The program developed is a C++ implementation of Banker's Algorithm to detect deadlock.

This program reads data from a file organized as following:
The file contains the number of processes, the number of resource types and  the allocation matrix.

#### Sample input file
```
5 3
10 5 7
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
7 5 3
2 2 2
9 0 2
2 2 2
4 3 3
3 3 2
```
data are separated by whitespace.

The first line has the number of processes and the number of resource types.
The next line has the total number of resources per process.
The next five line contains the allocation of resources to the processes.
Each line is a process.
The final five lines represenct the data for the request matrix. One line per each process.


### Compile
```
$ g++ -o bankers.o Bankers_algorithm.cpp

```
### Execute
```
Usage: ./bankers.o <input_file>
```
Two files input1.txt and input2.txt have been provided to check the possibility of deadlock or safe sequence.
For safe sequence, data are located in input2.txt whereas input1.txt is deadlock.

**Requirements**
- gcc

## Authors

#### Gift Nwatuzie

