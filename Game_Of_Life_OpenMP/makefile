all: basicompgol.c ompgol.c
	cc -o basicompgol basicompgol.c -fopenmp
	cc -o ompgol ompgol.c -fopenmp
clean:
	rm -f basicompgol
	rm -f ompgol