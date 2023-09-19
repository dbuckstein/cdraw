:: Visual Studio Launcher - cdraw (simplified plugin-only environment)
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER cdraw_LAUNCH_VS

set cdraw=%~dp0
call "%cdraw%\_dev\windows\cdraw_vslaunch_latest.bat" "cdrawPlugin"

::echo EXIT cdraw_LAUNCH_VS