#!/bin/bash
mkdir -p gen
mkdir -p bin
gcc gendata.c -O3 -o bin/gendata
gcc kadai10.c -O3 -o bin/main

for size in 1000 2000 4000 8000 16000 32000
do
    echo "-=-"
    echo "Data Size: $size"
    
    ./bin/gendata "$size" "gen/raw_$size"
    
    echo -n "Unsorted Time: "
    ./bin/main name "gen/raw_$size" "gen/sorted_$size"
    
    echo -n "Sorted Time:   "
    ./bin/main name "gen/sorted_$size" "gen/temp"
done