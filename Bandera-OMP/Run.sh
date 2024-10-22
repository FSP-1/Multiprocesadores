#!/bin/bash

#prevent threads migrating between cores
export export OMP_PROC_BIND=true
#set the number of threads
export OMP_NUM_THREADS=4

echo "Antes de compilar:"
echo "Secuencial: makefile sin -fopenmp"
echo "Paraleo:    makefile con -fopenmp" 

#######
make
time ./Bandera-OMP -r 1024 -c 1280 -o España 
eog España.png &
sleep 2
time ./Bandera-OMP -r 1200 -c 1920 -o España 

