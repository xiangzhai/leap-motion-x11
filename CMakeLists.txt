project(leap-motion-x11)

cmake_minimum_required(VERSION 2.8.11)

add_definitions(-std=c++11 -Werror -g)

find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(XTST REQUIRED xtst)
pkg_check_modules(LEAP REQUIRED libleap)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
