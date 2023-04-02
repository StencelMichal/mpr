filename="$1.csv"

rm $filename

values=10000000,50000000,100000000

for val in ${values//,/ }
do
    for threads in {1..4}
    do
        ./loop $threads $val >> $filename
    done
done
