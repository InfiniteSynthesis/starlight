#!/bin/bash
rm -rf build
mkdir build && cd build
cmake .. -G "Xcode"
xcodebuild
./bin/Debug/layout_test