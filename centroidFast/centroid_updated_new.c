#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include <stdalign.h>


void fill_array(double* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i] = (double) rand() / RAND_MAX; 

}


#define SIZE 105536





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







int main()
{
    double* a = (double*) calloc(SIZE, sizeof(double));
    double* b = (double*) calloc(SIZE, sizeof(double));
    double result;
    fill_array(a, SIZE);
    fill_array(b, SIZE);

    // Timing first option
    double t = omp_get_wtime();
    for (int k = 0; k < 1000; ++k)
        simple_loop1(a, b);
    printf("First loop took %g time\n", (omp_get_wtime()-t)/1000);

    // Timing second option
    t = omp_get_wtime();
    for (int k = 0; k < 1000; ++k)
        simple_loop2(a, b);
    printf("Second loop took %g time\n", (omp_get_wtime()-t)/1000);

    // Timing third option
    t = omp_get_wtime();
    for (int k = 0; k < 1000; ++k)
        simple_loop3(a, b);
    printf("Third loop took %g time\n", (omp_get_wtime()-t)/1000);




    // Timing fourth option
    struct data *ptr1, a_vec;
    struct data *ptr2, b_vec;

    ptr1 = &a_vec;
    ptr2 = &b_vec;

    fill_array(a_vec.vec, SIZE);
    fill_array(b_vec.vec, SIZE);


    t = omp_get_wtime();
    for (int k = 0; k < 1000; ++k)
        simple_loop4(ptr1, ptr2);
    printf("Fourth loop took %g time\n", (omp_get_wtime()-t)/1000);



    free(a);
    free(b);
}

