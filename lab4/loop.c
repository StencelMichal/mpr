#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Error: provide number of threads and array size\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int array_size = atoi(argv[2]);

    omp_set_num_threads(num_threads);

    double start, end;
    unsigned short seed[3];
    seed[0] = 0;
    seed[1] = 0;
    seed[2] = 0;
    double *numbers = calloc(array_size, sizeof(double));

    int i;
    double cpu_time_used;
    start = omp_get_wtime();
    #pragma omp parallel private(seed)
    {
        #pragma omp for schedule(auto)
            for(i = 0; i < array_size; i++){
                numbers[i] = erand48(seed);
            }
    }
    end = omp_get_wtime();
    cpu_time_used = (end - start);
    printf("%d;%d;%f\n", array_size, num_threads, cpu_time_used);
    return 0;
}

