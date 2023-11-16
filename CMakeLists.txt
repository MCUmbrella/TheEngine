cmake_minimum_required(VERSION 3.18)
project(TheEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SOURCES src/*)

# import SDL2
find_package(PkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

# import Lua
find_package(Lua 5.4 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

add_compile_options(-Wall -Wextra -Werror=return-type -Og -gdwarf-4 -g)
#add_compile_options(-Wall -O4)
#add_link_options(-static)
add_executable(TheEngine ${SOURCES})

Target_Link_Libraries (
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${LUA_LIBRARIES}
        m
)
