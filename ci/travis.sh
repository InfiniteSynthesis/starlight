cd ./layout_test/
export PATH="$PATH:`pwd`/Tools"

ruby ./gentest.rb

mkdir build
cd build
cmake .. -G "Xcode"
xcodebuild
./bin/Debug/layout_test