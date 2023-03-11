for i in {1..500}
do
    a=$(( 20000*i ))
    mpiexec -machinefile ./1node -np 2 ./ping_pong_send.py $a >> send1.csv
    mpiexec -machinefile ./2node -np 2 ./ping_pong_send.py $a >> send2.csv
    mpiexec -machinefile ./1node -np 2 ./ping_pong_ssend.py $a >> ssend1.csv
    mpiexec -machinefile ./2node -np 2 ./ping_pong_ssend.py $a >> ssend2.csv
done