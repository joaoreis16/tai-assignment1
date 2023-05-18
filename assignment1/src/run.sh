#!/bin/bash

make

echo $?

declare -a alpha=(1 0.75 0.50 0.25)


if [ $? == 2 ]; then
    echo "Exiting..."
else
    for K in {3..6}; do
        for A in "${alpha[@]}"; do
            for T in {1..3}; do
                f=$(printf "%s../results2/file%d_%d.txt" "" $K $T)
                if [ ! -e $f ]; then
                    echo $f
                    echo Executing CPM for K=$K alpha=$A threshold=$T
                    ./CPM -f ../example/chry.txt -k $K -a $A -t $T > ../results2/file${K}_${T}.txt
                fi
            done
        done
    done
fi