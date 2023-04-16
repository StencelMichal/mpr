#!/bin/bash

bucket_size_file="bucket_size.csv"
results="results.csv"

if [ -e "$bucket_size_file" ]; then
  rm "$bucket_size_file"
fi

if [ -e "$results" ]; then
  rm "$results"
fi

for ((i = 1; i <= 1; i += 1)); do
  ./sort.out 1 50000000 $i >> bucket_size.csv
done

#for ((i = 1; i <= 50; i += 1)); do
#  ./sort.out $i 50000000 $i >>"$results"
#done
