#!/bin/bash

gcc -Wall -g -o min.exe min.c
gcc -Wall -g -o max.exe max.c
gcc -Wall -g -o mean.exe mean.c
gcc -Wall -g -o median.exe median.c
gcc -Wall -g -o hist.exe hist.c
./calc_stat.sh input_files/044101
./calc_stat.sh input_files/044131
./calc_stat.sh input_files/044102
for j in {1..9}
    do
        for i in {1..1000}
        do
            echo "$(( ($(od -An -N2 -i /dev/random) )%(100) ))" >> "00000$j".txt
        done
    ./calc_stat.sh "00000$j"
    rm "00000$j".txt
done