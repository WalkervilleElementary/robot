@echo off
setlocal EnableDelayedExpansion enableextensions
setlocal

title Converting files to Arduino IDE format

set /p Arduino= What Directory would you like?

if not exist "!Arduino!\libraries\walkerville" mkdir "!Arduino!\libraries\walkerville"
if not exist "!Arduino!\sketches\robot" mkdir "!Arduino!\sketches\robot"

for /R %cd%/include %%a in (*) do (
 set filename=%%~nxa
 
 IF NOT "!filename!" == "configs.h" (
  xcopy /y %%a "!Arduino!\libraries\walkerville"
 )
)

for /R %cd%/src %%a in (*) do (
 set filename=%%~nxa
 
 IF NOT "!filename!" == "main.cpp" (
  xcopy /y %%a "!Arduino!\libraries\walkerville"
 )
)

echo f | xcopy /Y "!cd!\src\main.cpp" "!Arduino!\sketches\robot\robot.ino"


setlocal disableDelayedExpansion

set InputFile=%cd%\include\configs.h
set OutputFile=%Arduino%\libraries\walkerville\configs.h

(
  for /f "usebackq delims=" %%A in ("%InputFile%") do (
    if "%%A" == "#define DEVICE() 0" (echo #define DEVICE^(^) 1) else (echo %%A)
  )
) > "%OutputFile%"

pause
