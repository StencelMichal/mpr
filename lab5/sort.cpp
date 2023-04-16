#include <iostream>
#include <list>
#include <utility>
#include <vector>
//#include "/usr/local/opt/libomp/include/omp.h"
#include "omp.h"

using namespace std;


class Bucket {
public:
    list<double> numbers;

    void add(double val) {
        numbers.push_front(val);
    }

    void print() {
        for (auto it = numbers.begin(); it != numbers.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }

    void sort_values() {
        numbers.sort();
    }

    int size() {
        return numbers.size();
    }
};

struct Configuration {
    int num_threads;
    int array_size;
    int bucket_amount;
};

void print_array(double *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%f\n", array[i]);
    }
}

Configuration load_config_from_args(int argc, char *argv[]) {
    Configuration config{};
    if (argc < 4) {
        printf("Error: provide number of threads, array size and number of buckets\n");
        exit(1);
    }
    config.num_threads = atoi(argv[1]);
    config.array_size = atoi(argv[2]);
    config.bucket_amount = atoi(argv[3]);
    return config;
}

double *generate_random_numbers(int array_size) {
    unsigned short seed[3];
    seed[0] = 0;
    seed[1] = 0;
    seed[2] = 0;
    auto *numbers = static_cast<double *>(calloc(array_size, sizeof(double)));
    for (int i = 0; i < array_size; i++) {
        numbers[i] = erand48(seed);
    }
    return numbers;
}

void sort_buckets(vector<Bucket> &buckets) {
    for (auto &bucket: buckets) {
        bucket.sort_values();
    }
}

void assign_to_buckets(int thread_id, const double *numbers, Configuration config, vector<Bucket> buckets) {
    //dodać osobne czytanie
    double bucket_size = 1.0 / double(buckets.size());
    double from_value_range = thread_id * (1.0 / config.num_threads);
    double to_value_range = (thread_id + 1) * (1.0 / config.num_threads);
    printf("THreadId: %d, Bucket size: %d, From: %f, to: %f\n", thread_id, bucket_size, from_value_range,
           to_value_range);
#pragma omp for schedule(runtime)
    for (int i = 0; i < config.array_size; i++) {
        double number = numbers[i];
        if (from_value_range <= number && number < to_value_range) {
            int bucket_index = number / bucket_size;
            printf("Bucket index: %d\n", bucket_index);
            buckets[bucket_index].add(number);
        }
    }
}

int count_preceding_elements(vector<vector<Bucket>> buckets_by_thread, int thread_id) {
    int count = 0;
    for (int i = 0; i < thread_id; i++) {
        for (auto &bucket: buckets_by_thread[i]) {
            count += bucket.size();
        }
    }
    return count;
}

void reassign_to_array(vector<vector<Bucket>> buckets_by_thread, int thread_id, double *numbers) {
    int i = count_preceding_elements(buckets_by_thread, thread_id);
    printf("preceding elements: %d", i);
    vector<Bucket> buckets = buckets_by_thread[thread_id];
    for (auto &bucket: buckets) {
        printf("\nBucket\n");
        bucket.print();
        for (auto &number: bucket.numbers) {
            numbers[i] = number;
            i++;
        }
    }
    printf("\nArray after reassigning: \n)");
    print_array(numbers, 10);
}


double *sort(double *numbers, Configuration config) {
    vector<vector<Bucket>> buckets_by_thread;
    for (int i = 0; i < config.num_threads; i++) {
        buckets_by_thread.push_back(vector<Bucket>(config.bucket_amount));
    }
    omp_set_num_threads(config.num_threads);
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        vector<Bucket> buckets = buckets_by_thread[thread_id];
        assign_to_buckets(thread_id, numbers, config, buckets);
        sort_buckets(buckets);
        reassign_to_array(buckets_by_thread, thread_id, numbers);
    }
    return numbers;
}


int main(int argc, char *argv[]) {
    Configuration config = load_config_from_args(argc, argv);
    double *numbers = generate_random_numbers(config.array_size);
    print_array(numbers, config.array_size);
    double *sorted = sort(numbers, config);
    printf("\nSorted array:\n");
    print_array(sorted, config.array_size);
}
