cmake_minimum_required(VERSION 3.15)

# Projektname und Sprache
project(cpp-project
    VERSION 1.0.0
    DESCRIPTION "HSFD C++ Project"
    LANGUAGES CXX
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

# Executable erstellen
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Include-Verzeichnisse hinzufügen (falls benötigt)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
