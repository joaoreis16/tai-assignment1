#!/bin/bash

make

if [ $? == 2 ]; then
    echo "Exiting..."
else
    for K in {3..8}; do
        for A in [0.25, 0,50, 0,75, 1]; do
            for T in [1..6]; do
                echo Executing CPM for K=$K alpha=$A threshold=$T
                ./CPM -f ../example/chry.txt -k $K -a $A -t $T > ../results/chry_K${K}_A${A}_T${T}.txt
            done
        done
    done
fi

