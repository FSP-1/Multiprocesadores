#!/bin/bash

# Sequential

make Wa-tor

#Wa-tor -ni 1000 --ffmpeg
#Wa-tor -ni 1000 -d

echo "-----------------------------------------"
echo "Wa-tor-r 201 -c 201 -ni 10000 serie"
echo "-----------------------------------------"
time ./Wa-tor -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000

echo "-----------------------------------------"
echo "Wa-tor-r 300 -c 300 -ni 10000 serie"
echo "-----------------------------------------"
time ./Wa-tor -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000

# Parallel

make Wa-tor-PTh


echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 1 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 1

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 2 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 2

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 4 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 201 -c 201 -nf 10000 -ns 1000 -ni 10000 -nt 4



echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 1 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 1

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 2 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 2

echo "-----------------------------------------"
echo "-r 201 -c 201 -ni 10000 4 Thread"
echo "-----------------------------------------"
time ./Wa-tor-PTh -r 300 -c 300 -nf 20000 -ns 7000 -ni 10000 -nt 4



