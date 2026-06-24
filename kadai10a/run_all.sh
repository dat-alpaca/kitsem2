#!/bin/bash
mkdir -p gen
mkdir -p bin
gcc gendata.c -O3 -o bin/gendata
gcc kadai7.c  -O3 -o bin/kadai7
gcc kadai10.c -O3 -o bin/kadai10

echo "-----------------------------------------------------------------------------"
echo "| Size  | Kadai 7 (U)    | Kadai 7 (S)    | Kadai 10 (U)   | Kadai 10 (S)   |"
echo "-----------------------------------------------------------------------------"
for size in 10000 20000 30000 40000 50000 60000 70000 80000 90000 100000
do
    # Generate data
    ./bin/gendata "$size" "gen/raw_$size"
    ./bin/gendata "$size" "gen/raw_array_$size"
    sed -i "1i$size" "gen/raw_array_$size"

    echo -n "| "
    printf "%-6s" "$size"
    echo -n "| "

    # Kadai 7:
    printf "%-14s" "$(./bin/kadai7 name "gen/raw_array_$size" "gen/sorted_array_$size" | tr -d '\n')"
    echo -n " | "
    printf "%-14s" "$(./bin/kadai7 name "gen/sorted_array_$size" "gen/temp" | tr -d '\n')"
    echo -n " | "

    # Kadai 10:
    printf "%-14s" "$(./bin/kadai10 name "gen/raw_$size" "gen/sorted_$size" | tr -d '\n')"
    echo -n " | "
    printf "%-14s" "$(./bin/kadai10 name "gen/sorted_$size" "gen/temp" | tr -d '\n')"
    echo " |"
done

echo "-----------------------------------------------------------------------------"
