:: Visual Studio Launcher - cdraw (internal, locate and launch latest version)
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER cdraw_vslaunch_latest

:: locate devenv
call "%~dp0/_vslocate.bat"

set "cdraw_sln=%cdraw%project\VisualStudio\sln\%~1\%~1.sln"

::echo _vsdevenv: %_vsdevenv%
::echo cdraw_sln: %cdraw_sln%
"%_vsdevenv%" "%cdraw_sln%"

::echo EXIT cdraw_vslaunch_latest