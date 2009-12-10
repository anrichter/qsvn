#! /bin/sh

echo "Build QSvn for Release"
cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE="Release" $(dirname $0)/../src
make
ctest

