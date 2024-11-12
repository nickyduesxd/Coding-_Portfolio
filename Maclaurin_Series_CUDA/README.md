# Parallelism with CUDA - Approximating Natural Logarithms using the Maclaurin Series

Your task in this lab is to construct and run a parallel GPU program that calculate ln(y) using the Maclaurin Series using CUDA.

CUDA is part of Nvidia’s toolkit for building programs that run on Nvidia GPUs. It requires constructing a program that has two parts: one or more compute kernels that are offloaded to the GPU for parallel execution, and host code that runs on the CPU. You task here is to write a CUDA program that computes ln(y). Your program should meet the following specifications:

The program should be called lncuda.cu, and your makefile should generate an executable called lncuda. The program is compiled using nvcc, which is Nvidia’s tool for compiling C programs with CUDA extensions.
The program takes two command line arguments. The first is an integer that is the number of terms to do in the series expansion, and not coincidentally, the number of CUDA threads that should be used. The second is a real number between 1 and 2 inclusive that is the value for which the logarithm is to be calculated. Be sure to do bounds checking, especially since the series will diverge for values greater than 2.
The program should write the calculated logarithm to standard out. Note that you can check your results using one of the many calculators on the Internet, e.g., https://www.rapidtables.com/calc/math/Ln_Calc.html.