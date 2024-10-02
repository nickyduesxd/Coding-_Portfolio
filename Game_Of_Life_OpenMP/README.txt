#Game of Life with OpenMP

#Part 1: Basic Parallelization

Executable called "basicompgol"

In the pthreads solution to GOL, you did static domain composition by dividing the columns up across a specified number of threads, where each thread then computed all of the generations for its columns. The threads had to synchronize at the end of each generation using barriers so that all the threads stayed on the same generation, to ensure that only 1 thread displayed the grid, to swap the grid, and to update the halo.

Your task is to use the same basic structure but to implement the parallelism using OMP directives rather than pthreads.

#Part 2: Advance Parallelization

While the approach used in Part I results in a parallel version of GOL, it has relatively coarse-grain parallelization and fixes the number of threads. The philosophy of OMP, however, is to use thread parallelization more aggressively and more automatically. In this part, the task is to parallelize for loops and let OMP manage the number of threads and their allocation.

For this version, start with your sequential version of GOL (with any bugs from Lab 2 fixed, of course), and investigate parallelizing as many for loops as possible using the #pragma omp parallel for directive. Some can’t (see hints below) but many can. Recall that parallelizing a for loop here means that OMP will assign different iterations of the loop to different threads automatically.

The executable should be called “ompgol" and you should include a target in the combined makefile that will build the executable. The requirements are as follows.