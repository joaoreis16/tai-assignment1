#!/bin/bash

make

if [ $? == 2 ]; then
    echo "Exiting..."
else
    ./CPM
fi