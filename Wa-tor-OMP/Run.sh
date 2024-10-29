#!/bin/bash
#prevent threads migrating between cores
export OMP_PROC_BIND=true

make

./Wa-tor-OMP -ni 1000 -ffmpeg

echo "-----------------------------------------"
echo "-r 100 -c 100 -ni 10000 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Wa-tor-OMP -r 102 -c 102 -ni 10000

echo "-----------------------------------------"
echo "-r 100 -c 100 -ni 10000 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Wa-tor-OMP -r 102 -c 102 -ni 10000

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 2 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=2
time ./Wa-tor-OMP -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 4 Thread"
echo "-----------------------------------------"
#set the number of threads
export OMP_NUM_THREADS=4
time ./Wa-tor-OMP -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000



