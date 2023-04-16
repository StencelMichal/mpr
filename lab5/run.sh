#!/bin/bash -l
# Job configuration

#SBATCH --nodes 1
#SBATCH --ntasks 30
#SBATCH --time=00:03:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu

bucket_size_file="bucket_size.csv"
results="results.csv"

if [ -e "$bucket_size_file" ]; then
  rm "$bucket_size_file"
fi

if [ -e "$results" ]; then
  rm "$results"
fi

for ((i = 1; i <= 1; i += 1)); do
  ./sort.out 1 50000000 $i
done

#for ((i = 1; i <= 30; i += 1)); do
#  ./sort.out $i 50000000 $i >>"$results"
#done
