cmake_minimum_required (VERSION 3.0)

project(minesweeper_c)

set(SOURCES logic.c logic.h main.c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(SDL2 REQUIRED)
include_directories  (
    ${SDL2_INCLUDE_DIRS}
) 

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    SDL2::SDL2 # Required for Arch Linux SDL2 
               #   https://discourse.libsdl.org/t/arch-linux-cmake-find-package-sdl2-required-passes-but-doesnt-find-anything/24226/2
) 
