#!/bin/bash
mkdir -p gen
mkdir -p bin
gcc gendata.c -O3 -o bin/gendata
gcc kadai10.c -O3 -o bin/main

for size in 10000 20000 30000 40000 50000 60000 70000 80000 90000 100000
do
    echo "-=-"
    echo "Data Size: $size"
    
    ./bin/gendata "$size" "gen/raw_$size"
    
    echo -n "Unsorted Time: "
    ./bin/main name "gen/raw_$size" "gen/sorted_$size"
    
    echo -n "Sorted Time:   "
    ./bin/main name "gen/sorted_$size" "gen/temp"
done