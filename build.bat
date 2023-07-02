@echo off
cd build


call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
set PICO_SDK_PATH=C:\Users\eduar\Documents\pico-sdk
set PICO_TOOLCHAIN_PATH=C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\12.2 mpacbti-rel1\bin

cmake -G "MinGW Makefiles" ..
mingw32-make

pause