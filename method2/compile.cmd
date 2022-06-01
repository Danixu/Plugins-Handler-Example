@echo off

@echo off

echo "Compiling the Windows version of the library, using the VS compiler"
cl.exe /LD /DBUILD_LIB /std:c++17 /EHsc /obuild/bin/plugins/plugin1_w.dll /I..\common\include /Iinclude ..\common\src\export.cpp src\plugin1.cpp

pause