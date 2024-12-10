#!/bin/bash

make all

echo "-----------------------------------------"
echo "OperacionImg serie"
echo "-----------------------------------------"
time ./OperacionImg -r 20 -n 3

echo "-----------------------------------------"
echo "OperacionImg MPI"
echo "-----------------------------------------"
time mpirun -np 2 ./OperacionImg-MPI -r 20 -n 3

echo "-----------------------------------------"
echo "OperacionImg MPI"
echo "-----------------------------------------"
time mpirun -np 4 ./OperacionImg-MPI -r 20 -n 3