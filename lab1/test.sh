for i in {1..1000}
do
    a=$(( 100*i ))
    mpiexec -machinefile ./2node -np 1 ./ping_pong.py $a >> results_2.csv
done