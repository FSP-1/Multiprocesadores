#!/bin/bash
#prevent threads migrating between cores
#export export OMP_PROC_BIND=true

make


echo "-----------------------------------------"
echo "-mi 1e4 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -o

echo "-----------------------------------------"
echo "-mi 1e4 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 10000 -o


echo "-----------------------------------------"
echo "-mi 1e5 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -o

echo "-----------------------------------------"
echo "-mi 1e5 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Mandel-OMP -r 1024 -c 1024 -mx -2 -my -2 -sx 4 -sy 4 -mi 100000 -o
