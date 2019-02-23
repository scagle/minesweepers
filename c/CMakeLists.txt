cmake_minimum_required (VERSION 3.0)

project(minesweeper_c)

set(SOURCES logic.c logic.h graphics.c graphics.h global.h main.c)

# Grabbed CMAKE_MODULE_PATH manipulations and ./cmake/FindSDL2_ttf.cmake from:
#   https://stackoverflow.com/questions/37817717/sdl2-library-not-found-with-make-but-with-cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${minesweeper_c_SOURCE_DIR}/cmake")

# Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# SDL2 for drawing
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# SDL2 TTF for fonts
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR}) 

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf)
