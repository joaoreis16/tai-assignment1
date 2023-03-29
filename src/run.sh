#!/bin/bash

make

echo $?

alpha=(0.25 0.50 0.75 1)

if [ $? == 2 ]; then
    echo "Exiting..."
else
    for K in {3..8}; do
        for A in "${alpha[@]}"; do
            for T in {1..6}; do
                f=$(printf "%s../results/file%d_%d.txt" "" $K $T)
                if [ ! -e $f ]; then
                    echo $f
                    echo Executing CPM for K=$K alpha=$A threshold=$T
                    ./CPM -f ../example/chry.txt -k $K -a $A -t $T > ../results/file${K}_${T}.txt
                fi
            done
        done
    done
fi