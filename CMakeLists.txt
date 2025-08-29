cmake_minimum_required(VERSION 3.10)
project(GRAPHICS)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(src)

add_library(glad STATIC include/glad/glad.c)

find_package(glfw3 REQUIRED)

add_executable(main src/main.cpp)

add_executable(ngon src/ngon.cpp)

add_executable(flag src/flag.cpp)

add_executable(pacman src/pacman.cpp
        include/camera.h
        include/shader.h)

file(GLOB SHADER_FILES "src/shader/*")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shader)
foreach(SHADER_FILE ${SHADER_FILES})
  configure_file(${SHADER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/shader/ COPYONLY)
endforeach()

target_link_libraries(main PRIVATE glad glfw)
target_link_libraries(ngon PRIVATE glad glfw)
target_link_libraries(flag PRIVATE glad glfw)
target_link_libraries(pacman PRIVATE glad glfw)

add_executable(galaxy_wiz src/grader/galaxy_wiz.cpp)
add_executable(ccw_convex_area src/grader/ccw_convex_area.cpp)
add_executable(cp_centroid src/grader/cp_centroid.cpp)

