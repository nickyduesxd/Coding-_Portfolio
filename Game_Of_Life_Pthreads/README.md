#Parallelizing the Game of Life Using pThreads

Parallelizing the Game of Life Using pThreads
Your task is the write a parallel program implementing the Game of Life (GOL) in C using pThreads. The executable should be called "pargol," and you should submit a makefile with a default target that will build the executable. 

The following are the requirements for pargol:

#Arguments

pargol should take four arguments, all integers. The first two are the same as the sequential version of GOL, while the last two are the same as the final two arguments in Lab 4, i.e., the number of threads to use and the option to print the partitioning information.

number of generations to run the simulation
frequency for displaying the grid
number of threads
verbosity for debugging. If verbosity=2, then time it. When the fourth argument is greater than zero, each thread should print the following partitioning information:

tid 0   columns:    0:11    (12)
tid 1   columns:    12:22   (11)
tid 2   columns:    23:33   (11)
...

#Initialization
To initialize the grid, rather than taking in arguments as in Lab 2, we are going to ask the user and to make it easier, you can assume if number of rows given is 0, then the user wants to use a file.

num rows: 0
filename: matrix.txt
        
num rows: 5
num cols: 5
seed: 1

If user gives row and column number, then initialize the matrix using random 0 or 1 based on seed value. Recall if seed is less than 0, use time to set the seed.

#Updating
Same as for the sequential version of GOL.

On each generation, update each grid point applying the GOL rules as specified above.

#Stopping condition

Same as for the sequential version of GOL.

Stop the simulation after the number of generations specified in the command line have been executed.