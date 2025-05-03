@echo off
echo Using MSYS2 to compile and run the 3D windmill...

REM Path to MSYS2 bash
set BASH="C:\msys64\usr\bin\bash.exe"

REM Create a script to run in bash
echo echo "Installing required packages..." > build_script.sh
echo pacman -S --noconfirm mingw-w64-x86_64-freeglut >> build_script.sh
echo echo "Compiling the windmill program..." >> build_script.sh
echo cd /c/Users/HP/Documents/windmill3D >> build_script.sh
echo g++ -I/mingw64/include build/cmake.cpp -o build/Windmill3D.exe -L/mingw64/lib -lfreeglut -lopengl32 -lglu32 >> build_script.sh
echo if [ $? -eq 0 ]; then >> build_script.sh
echo     echo "Compilation successful!" >> build_script.sh
echo     echo "Copying freeglut.dll to build directory..." >> build_script.sh
echo     cp /mingw64/bin/freeglut.dll build/ >> build_script.sh
echo     echo "Running the windmill program..." >> build_script.sh
echo     cd build >> build_script.sh
echo     ./Windmill3D.exe >> build_script.sh
echo else >> build_script.sh
echo     echo "Compilation failed!" >> build_script.sh
echo     read -p "Press Enter to continue..." >> build_script.sh
echo fi >> build_script.sh

REM Run the script in MSYS2 MINGW64 bash
C:\msys64\mingw64.exe -c "bash build_script.sh"

echo MSYS2 compilation complete.
pause 