#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr23-cpu

module add scipy-bundle/2021.10-foss-2021b

for i in {1..100}
do
    a=$(( 50000*i ))
    mpiexec -machinefile ./1node -np 2 ./ping_pong_send.py $a >> send1.csv
    mpiexec -machinefile ./2node -np 2 ./ping_pong_send.py $a >> send2.csv
    mpiexec -machinefile ./1node -np 2 ./ping_pong_ssend.py $a >> ssend1.csv
    mpiexec -machinefile ./2node -np 2 ./ping_pong_ssend.py $a >> ssend2.csv
done
