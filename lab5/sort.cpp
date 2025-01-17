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
        // printf("dodaje\n");
        // printint numbers in bucket
        // print();
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

class TimeMeasurement {
public:
    double start;
    double end;

    double time_delta() const {
        return end - start;
    }
};

TimeMeasurement generate_numbers_time;
TimeMeasurement assign_to_buckets_time;
TimeMeasurement sort_buckets_time;
TimeMeasurement reassign_to_array_time;
TimeMeasurement total_time;

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

double *generate_random_numbers(double *numbers, int array_size, unsigned short seed[3]) {
    generate_numbers_time.start = omp_get_wtime();
    int thread_id = omp_get_thread_num();
    seed[0] = thread_id;
    seed[1] = thread_id + 1;
    seed[2] = thread_id + 2;
#pragma omp for schedule(static)
    for (int i = 0; i < array_size; i++) {
        numbers[i] = erand48(seed);
    }
    generate_numbers_time.end = omp_get_wtime();
//    if (omp_get_thread_num() == 0) {
//        printf("Generated numbers\n)");
//        print_array(numbers, array_size);
//    }
    return numbers;
}

void sort_buckets(vector<Bucket> &buckets) {
    sort_buckets_time.start = omp_get_wtime();
    for (int i = 0; i < buckets.size(); i++) {
        // printf("Sorting buckets\n");
        // buckets[i].print();
        buckets[i].sort_values();
    }
}

void assign_to_buckets(int thread_id, const double *numbers, Configuration config, vector<Bucket> &buckets) {
    //dodać osobne czytanie
    // printf("bucket size: %zu\n", buckets.size());
    double bucket_size = 1.0 / double(buckets.size());
    double from_value_range = thread_id * (1.0 / double(config.num_threads));
    double to_value_range = (thread_id + 1) * (1.0 / double(config.num_threads));
    int buckets_amount = buckets.size();
    // printf("THreadId: %d, Bucket size: %f, From: %f, to: %f\n", thread_id, bucket_size, from_value_range,
//    to_value_range);

    for (int i = 0; i < config.array_size; i++) {
        int id = (i + thread_id * (config.array_size / config.num_threads)) % config.array_size;
        double number = numbers[id];
        // printf("Checking number: %f %f %f\n", from_value_range, number, to_value_range);
        if (number == 0.0 && thread_id == 0) {
            buckets[0].add(number);
        }
        if (from_value_range < number && number <= to_value_range) {
            int bucket_index = number / buckets_amount;
            // printf("Bucket index: %d\n", bucket_index);
            buckets[bucket_index].add(number);
        }
    }
    assign_to_buckets_time.end = omp_get_wtime();
}

int count_preceding_elements(vector<vector<Bucket>> &buckets_by_thread, int thread_id) {
    int count = 0;
    for (int i = 0; i < thread_id; i++) {
        for (auto &bucket: buckets_by_thread[i]) {
            count += bucket.size();
        }
    }
    return count;
}

void reassign_to_array(vector<vector<Bucket>> &buckets_by_thread, int thread_id, double *numbers) {
    reassign_to_array_time.start = omp_get_wtime();
    int i = count_preceding_elements(buckets_by_thread, thread_id);
    // printf("preceding elements: %d", i);
    vector<Bucket> buckets = buckets_by_thread[thread_id];
    for (auto &bucket: buckets) {
        // printf("\nBucket\n");
//        bucket.print();
        for (auto &number: bucket.numbers) {
            numbers[i] = number;
            i++;
        }
    }
    // printf("\nArray after reassigning: \n)");
//    print_array(numbers, 10);
    reassign_to_array_time.end = omp_get_wtime();
}


double *sort(Configuration config) {
    unsigned short seed[3];
    vector<vector<Bucket>> buckets_by_thread;
    int buckets_per_thread = config.bucket_amount / config.num_threads;
    for (int i = 0; i < config.num_threads - 1; i++) {
        buckets_by_thread.push_back(vector<Bucket>(buckets_per_thread));
    }
    buckets_by_thread.push_back(vector<Bucket>(buckets_per_thread + config.bucket_amount % config.num_threads));
    omp_set_num_threads(config.num_threads);
    auto *numbers = static_cast<double *>(calloc(config.array_size, sizeof(double)));
    total_time.start = omp_get_wtime();
#pragma omp parallel private(seed) shared(buckets_by_thread, numbers)
    {
        numbers = generate_random_numbers(numbers, config.array_size, seed);
        assign_to_buckets_time.start = omp_get_wtime();
        int thread_id = omp_get_thread_num();
        vector<Bucket> buckets = buckets_by_thread[thread_id];
        assign_to_buckets(thread_id, numbers, config, buckets);
        sort_buckets(buckets);
        buckets_by_thread[thread_id] = buckets;
#pragma omp barrier
        sort_buckets_time.end = omp_get_wtime();
        reassign_to_array(buckets_by_thread, thread_id, numbers);
    }
    total_time.end = omp_get_wtime();
    return numbers;
}

void print_time_measurements(Configuration config) {
    printf("1;%d;%d;%d;%f;%f;%f;%f;%f\n", config.array_size, config.bucket_amount, config.num_threads,
           total_time.time_delta(), generate_numbers_time.time_delta(), assign_to_buckets_time.time_delta(),
           sort_buckets_time.time_delta(), reassign_to_array_time.time_delta());
}


int main(int argc, char *argv[]) {
    Configuration config = load_config_from_args(argc, argv);
    double *sorted = sort(config);
//    printf("\nSorted array:\n");
//    print_array(sorted, config.array_size);
    print_time_measurements(config);
}
