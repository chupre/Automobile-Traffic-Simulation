#!/bin/bash

# Building
cmake -S . -B build
cd build
make

# Running
./Automobile-Traffic-Simulation

