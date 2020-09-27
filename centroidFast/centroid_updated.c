#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <stdalign.h>


#define SIZE 65536

void simple_loop1(double* a, double* b)
{
    for (int i = 0; i < SIZE; ++i)
        a[i] += b[i];
}

void simple_loop2(double* restrict a, double* restrict b)
{
    for (int i = 0; i < SIZE; ++i)
        a[i] += b[i];
}


#define GCC_ALN(var, alignment) \
	__builtin_assume_aligned(var, alignment)

void simple_loop3(double* restrict a, double* restrict b)
{
    a = GCC_ALN(a, 32);
    b = GCC_ALN(b, 32);
    for (int i = 0; i < SIZE; ++i)
        a[i] += b[i];
}


struct data {
	alignas(32) double vec[SIZE];
};

void simple_loop4(struct data* restrict a, struct data* restrict b)
{
    for (int i = 0; i < SIZE; ++i)
        a->vec[i] += b->vec[i];
}







void fill_array(double* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i] = (double) rand() / RAND_MAX;
}



int main()
{
    int n = 1000000;
    double* xy = (double*) calloc(2 * n, sizeof(double));





    double result[2];
    fill_array(xy, 2*n);

    // Timing first option
    double t = omp_get_wtime();
    for (int k = 0; k < 100; ++k)
        simpleloop1(result, xy, n);
    printf("%g (%g, %g)\n", (omp_get_wtime()-t)/100, result[0], result[1]);

    // Timing second option
    t = omp_get_wtime();
    for (int k = 0; k < 100; ++k)
        centroid2(result, xy, n);
    printf("%g (%g, %g)\n", (omp_get_wtime()-t)/100, result[0], result[1]);

    // Timing third option
    t = omp_get_wtime();
    for (int k = 0; k < 100; ++k)
        centroid3(result, xy, xy+n, n);
    printf("%g (%g, %g)\n", (omp_get_wtime()-t)/100, result[0], result[1]);

    free(xy);
}
