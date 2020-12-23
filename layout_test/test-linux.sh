#!/bin/bash
rm -rf build
mkdir build && cd build
cmake ..
make
./bin/layout_test