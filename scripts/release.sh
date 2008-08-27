#! /bin/sh

echo "Build QSvn for Release"
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE="Release" ../src
make
ctest

