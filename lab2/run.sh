#!/bin/bash

# Job configuration

#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu

# Variables
#number_of_points="5000000 41833001 350000000"
number_of_points="5000 41833 350000"
# Load modules
module add scipy-bundle/2021.10-foss-2021b

# pi calculation
run_pi_calculation() {
    local num_points=$1
    local num_processors=$2
    mpiexec -np "$num_processors" ./pi.py "$num_points"
}

# Loop through number of points and run pi calculation
for num_points in $number_of_points; do
    for ((i=1; i<=15; i++)); do
        for ((num_processors=1; num_processors<=12; num_processors++)); do
            run_pi_calculation "$num_points" "$num_processors"
        done
    done
done