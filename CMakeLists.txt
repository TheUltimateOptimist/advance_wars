cmake_minimum_required(VERSION 3.15)

# Projektname und Sprache
project(ADVANCED_WARS
    VERSION 1.0.0
    DESCRIPTION "HSFD C++ Project"
    LANGUAGES CXX
)

set(ADVANCED_WARS_SOURCES
    src/scene.hpp
    src/unit.cpp
    src/building.cpp
    src/level.cpp
    src/engine.cpp
    src/window.cpp
)

# C++ Standard festlegen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-Warnungen aktivieren
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# CMake
set(CMAKE_MODULE_PATH ${ADVANCED_WARS_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMG_INCLUDE_DIR})

# Executable erstellen
add_executable(advanced_wars src/main.cpp ${ADVANCED_WARS_SOURCES})
target_link_libraries(advanced_wars ${SDL2_LIBRARY} ${SDL2_IMG_LIBRARY})
target_link_libraries(advanced_wars m)
