cmake -G "NMake Makefiles" -D CMAKE_BUILD_TYPE="Release" ..\src 
nmake 
ctest 
nmake setup