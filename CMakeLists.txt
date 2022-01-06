# CMakeList.txt : CMake project for software_renderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("software_renderer")

# Add source to this project's executable.
add_executable (software_renderer "main.cpp"  "window.h" "window.cpp" "frame.h" "frame.cpp" "libs/stb_image.h" "utils.h" "utils.cpp")

# TODO: Add tests and install targets if needed.
