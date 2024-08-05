cmake_minimum_required(VERSION 3.10)
project(AsciiCamera)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Specify the path to the OpenCV installation
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

# Find OpenCV package
find_package(OpenCV REQUIRED)
set(CMAKE_BUILD_TYPE Debug) # or Release

if(OpenCV_FOUND)
    # Add the executable
    add_executable(AsciiCamera cpp/main.cpp)

    # Include OpenCV directories
    target_include_directories(AsciiCamera PRIVATE ${OpenCV_INCLUDE_DIRS})
    
    # Include your project's include directory
    target_include_directories(AsciiCamera PRIVATE ${CMAKE_SOURCE_DIR}/)

    # Link OpenCV libraries
    target_link_libraries(AsciiCamera PRIVATE ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV not found")
endif()
