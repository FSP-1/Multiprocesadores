#!/bin/bash


#Set rows to sequential lasts at least some seconds.
export Rows=20000
export Cols=20000
make Test

echo "-----------------------------------------"
echo "SumVectorInt serie -r 4"
echo "-----------------------------------------"
time ./Test -r 10 -c 10

make Test-OMP

export OMP_NUM_THREADS=4 

echo "-----------------------------------------"
echo "SumVectorInt-OMP -r 4"
echo "-----------------------------------------"
time ./Test-OMP -r 10 -c 10

make Test-PTH

echo "-----------------------------------------"
echo "SumVectorInt-PTh -r 4"
echo "-----------------------------------------"
time ./Test-PTH  -r 10 -c 10  -nt 4

#======================================================

echo "-----------------------------------------"
echo "SumVectorInt serie -r $Rows"
echo "-----------------------------------------"
time ./Test -r $Rows -c $Cols

echo "-----------------------------------------"
echo "SumVectorInt-OMP -r $Rows"
echo "-----------------------------------------"
export OMP_NUM_THREADS=4 
time ./Test-OMP  -r $Rows -c $Cols

echo "-----------------------------------------"
echo "SumVectorInt-PTh -r $Rows"
echo "-----------------------------------------"
time ./Test-PTH -r $Rows -c $Cols -nt 4
make clean