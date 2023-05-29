#!/bin/bash

a_min=0.1
a_max=1
a_step=0.1

t_min=4
t_max=10
t_step=1

k_min=4
k_max=10
k_step=1


for a in $(seq $a_min $a_step $a_max | tr ',' '.'); do
  for t in $(seq $t_min $t_step $t_max); do
    for k in $(seq $k_min $k_step $k_max); do
      echo "Running findlang.cpp with a=$a, t=$t, k=$k"
      ./findlang lang_files/short target_files/Dutch.utf8 -a $a -t $t -k $k
    done
  done
done

for a in $(seq $a_min $a_step $a_max | tr ',' '.'); do
    for k in $(seq $k_min $k_step $k_max); do
        echo "Running findlang.cpp with a=$a, k=$k"
        ./findlang lang_files/short target_files/Dutch.utf8 -a $a -k $k -f
    done
done