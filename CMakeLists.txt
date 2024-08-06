cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug) # or Release
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)


project(AsciiCamera)

# Find OpenCV package
find_package(OpenCV CONFIG REQUIRED)

# Add the executable
add_executable(AsciiCamera cpp/main.cpp)

# Link OpenCV libraries
target_link_libraries(AsciiCamera PRIVATE opencv_ml opencv_core opencv_flann opencv_videoio opencv_highgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
