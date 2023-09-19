:: Windows utility script to copy file.
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER _copy

set "srcpath=%~1"
set "dstpath=%~2"
set "srcname=%~3"
set "dstname=%~4"

xcopy /y "%srcpath%%srcname%" "%dstpath%%dstname%"

::echo EXIT _copy