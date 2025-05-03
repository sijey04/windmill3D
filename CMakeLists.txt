cmake_minimum_required(VERSION 3.10)
project(Windmill3D)

# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Message about packages
message(STATUS "OpenGL include dirs: ${OPENGL_INCLUDE_DIRS}")
message(STATUS "GLUT include dirs: ${GLUT_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLUT libraries: ${GLUT_LIBRARIES}")

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Define executable
add_executable(Windmill3D build/cmake.cpp)

# Link libraries
target_link_libraries(Windmill3D 
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Copy any necessary resources or DLLs
if(WIN32)
    # Check if we're using vcpkg
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")
        message(STATUS "vcpkg detected, configuring accordingly.")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
    
    # Location of DLLs for Windows
    if(EXISTS "${GLUT_INCLUDE_DIR}/../bin")
        file(GLOB GLUT_DLLS "${GLUT_INCLUDE_DIR}/../bin/*.dll")
        foreach(DLL ${GLUT_DLLS})
            message(STATUS "Will copy DLL: ${DLL}")
            add_custom_command(TARGET Windmill3D POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL} $<TARGET_FILE_DIR:Windmill3D>)
        endforeach()
    endif()
endif()