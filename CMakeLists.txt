cmake_minimum_required (VERSION 3.7)
project (software_renderer)



file(GLOB source_files "src/*.h" "src/*.cpp" "libs/*.h")
add_executable(software_renderer ${source_files} "src/scene.h" "src/scene.cpp" "src/shader.h")
# move files to build
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_custom_command(TARGET software_renderer PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:software_renderer>)