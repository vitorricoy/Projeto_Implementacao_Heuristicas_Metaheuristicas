#!/bin/bash
file="*.vrp"
for dir in "A" "B" "DIMACS" "E" "F"; do
    for filename in ../test/$dir/$file; do
        fbname=$(basename "$filename" | cut -d. -f1)
        ./Program/genvrp $filename sol/$dir/$fbname.sol;
    done
done
