:: Windows utility script to copy build files - cdraw.
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER cdraw_copybuild

set "libpath=%1"
set "binpath=%2"
set "libname=%3"
set "binname=%4"
set "datapath=%5"

:: create output directory in case it does not exist yet
:: write to dummy file and perform copy
:: also create asset data directory
cd "%cdraw%"
mkdir %binpath%
mkdir %datapath%
set dstfile="%~2\%~4"
@echo cdraw>%dstfile%
call ".\_dev\windows\_copy.bat" %libpath% %binpath% %libname% %binname%

::echo EXIT cdraw_copybuild