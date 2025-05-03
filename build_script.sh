echo "Installing required packages..." 
pacman -S --noconfirm mingw-w64-x86_64-freeglut 
echo "Compiling the windmill program..." 
cd /c/Users/HP/Documents/windmill3D 
g++ -I/mingw64/include build/cmake.cpp -o build/Windmill3D.exe -L/mingw64/lib -lfreeglut -lopengl32 -lglu32 
if [ $? -eq 0 ]; then 
    echo "Compilation successful!" 
    echo "Copying freeglut.dll to build directory..." 
    cp /mingw64/bin/freeglut.dll build/ 
    echo "Running the windmill program..." 
    cd build 
    ./Windmill3D.exe 
else 
    echo "Compilation failed!" 
    read -p "Press Enter to continue..." 
fi 
