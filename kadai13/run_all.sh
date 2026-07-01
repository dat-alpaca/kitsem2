#!/bin/bash
mkdir -p gen
mkdir -p bin
gcc gendata.c -O3 -o bin/gendata
gcc _kadai9_timed.c  -O3 -o bin/kadai9
gcc _kadai12_timed.c -O3 -o bin/kadai12

echo "-----------------------------------------------------------------------------"
echo "| Size  | Kadai 9 (U)    | Kadai 9 (S)    | Kadai 12 (U)   | Kadai 12 (S)   |"
echo "-----------------------------------------------------------------------------"
for size in 10000 25000 50000 100000
do
    # Generate data
    ./bin/gendata "$size" "gen/raw_$size"
    ./bin/gendata "$size" "gen/raw_array_$size"
    sed -i "1i$size" "gen/raw_array_$size"

    echo -n "| "
    printf "%-6s" "$size"
    echo -n "| "

    # Kadai 9:
    printf "%-14s" "$(./bin/kadai9 name "gen/raw_$size" "gen/sorted_9_$size" | tr -d '\n')"
    echo -n " | "
    printf "%-14s" "$(./bin/kadai9 name "gen/sorted_9_$size" "gen/temp" | tr -d '\n')"
    echo -n " | "

    # Kadai 12:
    printf "%-14s" "$(./bin/kadai12 name "gen/raw_$size" "gen/sorted_12_$size" | tr -d '\n')"
    echo -n " | "
    printf "%-14s" "$(./bin/kadai12 name "gen/sorted_12_$size" "gen/temp" | tr -d '\n')"
    echo " |"
done

echo "-----------------------------------------------------------------------------"
