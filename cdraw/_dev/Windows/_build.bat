:: Windows utility script to build project.
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER _build

set launchpath=%1
set buildproj=%2
set buildcfg=%3
set buildlog=%4
set buildsw=%~5
set stopcopy=%~6

call %launchpath% %buildproj% /%buildsw% %buildcfg% /Out %buildlog%

::echo EXIT _build