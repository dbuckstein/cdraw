:: Visual Studio Launcher - cdraw (simplified math testing environment)
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER cdraw_LAUNCH_VS

set cdraw=%~dp0
call "%cdraw%\_dev\windows\cdraw_vslaunch_latest.bat" "cdrawMath"

::echo EXIT cdraw_LAUNCH_VS