#!/bin/bash

make

alpha=(0.25 0.50 0.75 1)

if [ $? == 2 ]; then
    echo "Exiting..."
else
    for K in {3..8}; do
        for A in "${floats[@]}"; do
            for T in [1..6]; do
                f=$(printf "%../results/%d_%d_%d.txt" $K $A $T)
                if [ ! -e $f ]; then
                    echo $f
                    echo Executing CPM for K=$K alpha=$A threshold=$T
                    ./CPM -f ../example/chry.txt -k $K -a $A -t $T > ../results/chry_K${K}_A${A}_T${T}.txt
                fi
            done
        done
    done
fi

