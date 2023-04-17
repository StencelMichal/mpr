#!/bin/bash -l
# Job configuration

#SBATCH --nodes 1
#SBATCH --ntasks 30
#SBATCH --time=00:30:00
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

#for ((i = 0; i <= 1000000; i += 1000)); do
#  if [ "$i" -eq 0 ]; then
#    i=1
#  fi
#  ./sort.out 1 1000000 $i
#done
#
for ((i = 1; i <= 30; i += 1)); do
  ./sort.out $i 1000000 10000
done
