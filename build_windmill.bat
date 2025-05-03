@echo off
echo Building Windmill3D...
echo Created by Christian Jude Faminiano, Marie Andre Abao, Mosses Ramos, Matthew Lim, Jomark Abello BSCS-3C

REM Set paths to libraries - Updated for MSYS2
set INCLUDE_PATH=-I"C:\msys64\mingw64\include"
set LIB_PATH=-L"C:\msys64\mingw64\lib"

REM Use full path to g++
set GXX="C:\msys64\mingw64\bin\g++.exe"

echo Compiling with command:
echo %GXX% %INCLUDE_PATH% build\cmake.cpp -o build\Windmill3D.exe %LIB_PATH% -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++

REM Compile the program
%GXX% %INCLUDE_PATH% build\cmake.cpp -o build\Windmill3D.exe %LIB_PATH% -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++

if %ERRORLEVEL% EQU 0 (
  echo Build successful! 
  echo Running Windmill3D...
  
  REM Copy the DLL if it doesn't exist in the build directory
  if not exist "build\freeglut.dll" (
    echo Copying freeglut.dll to build directory...
    copy "C:\msys64\mingw64\bin\freeglut.dll" "build\"
  )
  
  echo.
  echo =============================================================
  echo Windmill3D Controls:
  echo - Click the blue "Auto-Rotate" button to toggle camera rotation
  echo - Use arrow keys to manually rotate the camera (when auto-rotate is off)
  echo - A/Z keys: Zoom in/out
  echo - W/S keys: Adjust camera height
  echo - Space: Toggle auto-rotation
  echo - H: Toggle help display
  echo - ESC: Exit program
  echo =============================================================
  echo.
  
  cd build
  Windmill3D.exe
) else (
  echo Build failed with error code %ERRORLEVEL%.
  echo Make sure you have installed all the required libraries using MSYS2:
  echo pacman -S mingw-w64-x86_64-gcc
  echo pacman -S mingw-w64-x86_64-freeglut
  pause
) 