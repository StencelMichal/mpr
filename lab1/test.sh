for i in {1..1000}
do
    a=$(( 10000*i ))
    mpiexec -machinefile ./2node -np 2 ./ping_pong.py $a >> results_2.csv
done