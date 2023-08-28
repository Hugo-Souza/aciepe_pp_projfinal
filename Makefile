CC=gcc

all: clean sequential openmp

sequential:
	$(CC) laplace_seq.c -o laplace_seq

openmp:
	$(CC) laplace_omp.c -o laplace_omp -fopenmp

clean:
	rm -f laplace_seq laplace_omp
