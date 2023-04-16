#!/bin/bash

# Usuwanie 3 plików, jeśli istnieją
if [ -e plik1.txt ]
then
  rm plik1.txt
fi

if [ -e plik2.txt ]
then
  rm plik2.txt
fi

if [ -e plik3.txt ]
then
  rm plik3.txt
fi

# Pętla od 1 do 50000000 co 500000 uruchamiająca skrypt
for ((i=1; i<=50000000; i+=500000))
do
  ./nazwa_skryptu.sh 1 50000000 $i >> plik1.txt
done
