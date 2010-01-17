cmake -G "NMake Makefiles" -D CMAKE_BUILD_TYPE="Release" %* %~d0%~p0..\src 
nmake all test setup
