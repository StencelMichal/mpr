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
run_pi_calculation 1280000000 12


