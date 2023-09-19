:: Windows utility script to call build copy - cdraw.
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER cdraw_copybuild_call

set "calldir=%~dp0"
set "calllibpath=%1"
set "callbinpath=%2"
set "calllibname=%3"
set "callbinname=%4"
set "calldatapath=%5"

if not "%stopcopy%"=="YES" (
	call "%calldir%cdraw_copybuild.bat" %calllibpath% %callbinpath% %calllibname% %callbinname% %calldatapath%
) else (
	echo cdraw deferred copy build...
)

::echo ENTER cdraw_copybuild_call