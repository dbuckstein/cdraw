:: Version Directory Locator
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER _verlocate

set dir=%~1
set rt=%~2

:: echo %dir% %rt%
:: Highest alphanumeric directory is set as %ver% at the end of this program.
for /d /r "%rt%" %%i in (%dir%*) do (
	if exist "%%i" (
		set ver=%%i
	)
)

set dir=
set rt=

::echo EXIT _verlocate