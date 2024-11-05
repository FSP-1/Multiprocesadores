#!/bin/bash

#Serie=========================================
make Bandera

echo "-----------------------------------------"
echo "Serie: -r 1024 -c 1024 "
echo "-----------------------------------------"
time ./Bandera -r 1024 -c 1024
#eog España.png &

echo "-----------------------------------------"
echo "Serie: -r 2048 -c 2048"
echo "-----------------------------------------"
time ./Bandera -r 2048 -c 2048 


#PTh============================================
make Bandera-PTh

echo "-----------------------------------------"
echo "PTh: -r 1024 -c 1024 "
echo "-----------------------------------------"
time ./Bandera-PTh -r 1024 -c 1024 -nt 1
time ./Bandera-PTh -r 1024 -c 1024 -nt 2
time ./Bandera-PTh -r 1024 -c 1024 -nt 4

echo "-----------------------------------------"
echo "PTh: -r 2048 -c 2048"
echo "-----------------------------------------"
time ./Bandera-PTh -r 2048 -c 2048 -nt 1 
time ./Bandera-PTh -r 2048 -c 2048 -nt 2
time ./Bandera-PTh -r 2048 -c 2048 -nt 4


