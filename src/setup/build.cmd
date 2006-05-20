for /F "tokens=2,3 delims= " %%i in (..\version.h) do set %%i=%%j
candle qsvn.wxs
light -out qsvn-%QSVN_MAJOR_VERSION%.%QSVN_MINOR_VERSION%.%QSVN_MICRO_VERSION%.msi qsvn.wixobj
