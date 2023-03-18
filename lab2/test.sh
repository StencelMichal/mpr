#!/bin/bash

# Job configuration

#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu

module add scipy-bundle/2021.10-foss-2021b

# pi calculation
run_pi_calculation() {
    local num_points=$1
    local num_processors=$2
    mpiexec -np "$num_processors" ./pi.py "$num_points"
}

run_pi_calculation 1000000 1
run_pi_calculation 2000000 1
run_pi_calculation 4000000 1

run_pi_calculation 10000000 12
run_pi_calculation 20000000 12
run_pi_calculation 40000000 12
run_pi_calculation 80000000 12
run_pi_calculation 160000000 12
run_pi_calculation 320000000 12

