:: GLEW Locator (internal, locate OpenGL Extension Wrangler)
:: Copyright 2023 Daniel S. Buckstein
@echo off

::echo ENTER _glewlocate

call "_verlocate" "glew" "C:\GLEW"
if not "%ver%"=="" (
	::echo %ver%
	setx GLEW_PATH %ver%
	echo _glewlocate succeeded
) else (
	echo _glewlocate failed
)

::echo EXIT _glewlocate