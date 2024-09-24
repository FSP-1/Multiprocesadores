#!/bin/bash
make
time ./Bandera -r 1024 -c 1024 -o España 
eog España.png &
sleep 2
time ./Bandera -r 400 -c 600 -o España 
