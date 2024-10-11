#!/bin/bash 
make 
#Wator without graphical output neither generating data to gnuplot
time ./Wa-tor 

#Wator with graphical output (10 iterations) and without generating data to gnuplot
#Uncomment next line to run on console. 
#time ./Wa-tor -o -ni 10

#Wator without graphical output but generating data to gnuplot
#Uncomment next line to run on console.
#time ./Wa-tor -d 

#Wator with graphical output (10 itrations) and generating data to gnuplot
#Uncomment next line to run on console.
#time ./Wa-tor -o -d -ni 10

#Wator with viedo output of 1000 iterations
#Uncomment next line to run on console.
#time ./Wa-tor -ni 1000 -ffmpeg


