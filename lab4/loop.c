#include <stdio.h>
#include <omp.h>

int main(){

    double start, end;
    unsigned short seed[3] = {1, 2, 3};
    double numbers[100];
    int i;
    double cpu_time_used;
    start = omp_get_wtime();
    #pragma omp parallel private(seed)
    {
        #pragma omp for
            for(i = 0; i < 100; i++){
                numbers[i] = erand48(seed);
                printf("number: %f %f\n", numbers[i], erand48(seed));
            }
    }
    end = omp_get_wtime();
    cpu_time_used = (end - start);
    printf("%f \n", cpu_time_used);
    return 0;
}

