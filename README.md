# 3D Windmill Animation

This project demonstrates a 3D windmill animation using OpenGL and GLUT. The animation features a rotating windmill with multiple blades on a green landscape with a blue sky background.

## Created By
- Christian Jude Faminiano
- Marie Andre Abao
- Mosses Ramos
- Matthew Lim
- Jomark Abello

## Features

- 3D windmill model with rotating blades
- Realistic wooden tower
- Auto-rotating camera view for a dynamic perspective
- Simulated grass-like ground and sky blue background
- Smooth animation using timer-based updates
- Interactive camera controls
- On-screen controls and help display

## Prerequisites

### Windows with MSYS2 (Recommended)

1. **Install MSYS2**:
   - Download and install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)
   - Follow the installation instructions on the website

2. **Install Required Packages**:
   - Open MSYS2 MinGW 64-bit terminal
   - Run the following commands:
     ```
     pacman -Syu
     pacman -S mingw-w64-x86_64-gcc
     pacman -S mingw-w64-x86_64-freeglut
     ```

### Windows with MinGW (Alternative)

1. **C++ Compiler**:
   - Install MinGW with GCC: [Download](https://www.mingw-w64.org/downloads/)
   - Add MinGW's bin directory to your system PATH

2. **FreeGLUT Library**:
   - Download from [FreeGLUT website](https://www.transmissionzero.co.uk/software/freeglut-devel/)
   - Extract and copy:
     - `.dll` files to your Windows System32 folder or your project's build folder
     - `.lib` files to your MinGW lib folder
     - Header files to your MinGW include/GL folder

### macOS

- Install with Homebrew:
  ```
  brew install freeglut
  ```

### Linux

- Debian/Ubuntu:
  ```
  sudo apt-get install freeglut3-dev
  ```
- Fedora:
  ```
  sudo dnf install freeglut-devel
  ```

## Building and Running

### Using build_windmill.bat (Easiest Method)

1. Make sure you've installed all prerequisites (MSYS2 with MinGW64 and FreeGLUT)
2. Simply double-click on `build_windmill.bat` in the project folder
3. The script will:
   - Compile the program
   - Copy any necessary DLL files
   - Run the windmill animation automatically

If you encounter any issues, the script will display helpful error messages and installation guidance.

### Using Visual Studio Code

1. Ensure you have the C/C++ extension installed in VS Code
2. Open the project folder in VS Code
3. Press `Ctrl+Shift+B` to build the project (this will run the CMake tasks)
4. Press `F5` to run and debug the application

### Using CodeBlocks

1. Open CodeBlocks and go to File > Open
2. Navigate to the project folder and open the source file (`build/cmake.cpp`)
3. Go to Project > Build Options and add the following libraries:
   - freeglut
   - opengl32
   - glu32
4. Click Build and Run

### Using Command Line

```bash
# Create and navigate to build directory
mkdir -p build
cd build

# Generate build files with CMake
cmake .. -G "MinGW Makefiles"  # For Windows with MinGW
# OR
cmake ..                       # For Linux/macOS

# Build the project
cmake --build .

# Run the executable
./OpenGLProject  # Linux/macOS
# OR
OpenGLProject.exe  # Windows
```

## Controls

The application has both on-screen controls and keyboard/mouse interaction:

### Camera Controls

- **Auto-Rotation**: Click the blue "Auto-Rotate" button in the top-left corner to toggle
- **Manual Camera Rotation**:
  - Left/Right Arrow Keys: Rotate camera around the windmill
  - Mouse Drag: Click and drag horizontally to rotate the camera (when auto-rotate is off)
- **Camera Position**:
  - A/Z Keys: Zoom in/out
  - W/S or Up/Down Arrow Keys: Adjust camera height
  - R Key: Reset camera to default position

### General Controls

- **Space Bar**: Toggle camera auto-rotation
- **H Key**: Toggle help/controls display
- **ESC Key**: Exit the program

The windmill blades automatically rotate regardless of camera settings.

## Troubleshooting

If you encounter any issues when running with build_windmill.bat:
- Make sure MSYS2 is installed correctly
- Verify that you've installed all required packages with pacman
- Check that the paths in build_windmill.bat match your MSYS2 installation path

If you encounter any issues with missing libraries:
- Check that GLUT is properly installed and in your system PATH
- Make sure your compiler is correctly installed and in your system PATH
- Verify that the paths in `.vscode/launch.json` match your actual installation paths

For Windows users experiencing issues with FreeGLUT:
- Try using the `-DFREEGLUT_STATIC=ON` flag when configuring CMake
- Ensure you have the OpenGL headers and libraries (often included with graphics drivers)
- Copy freeglut.dll to the same directory as your executable

## Customization

You can modify various aspects of the windmill by adjusting the constants at the top of the source file:

- `NUM_BLADES`: Number of windmill blades
- `BLADE_LENGTH` and `BLADE_WIDTH`: Size of each blade
- `TOWER_HEIGHT` and `TOWER_RADIUS`: Dimensions of the windmill tower
- `ROTATION_SPEED`: Speed of blade rotation
- `CAMERA_ROTATION_SPEED`: Speed of auto-rotation for the camera

## License

This project is available under the MIT License. Feel free to use, modify, and distribute as needed.