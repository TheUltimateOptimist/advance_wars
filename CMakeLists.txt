cmake_minimum_required(VERSION 3.15)

# Projektname und Sprache
project(ADVANCED_WARS
        VERSION 1.0.0
        DESCRIPTION "HSFD C++ Project"
        LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
    highfive
    GIT_REPOSITORY https://github.com/highfive-devs/highfive.git
    GIT_TAG v2.10.1
)

set(HIGHFIVE_EXAMPLES OFF)
set(HIGHFIVE_UNIT_TESTS OFF)

FetchContent_MakeAvailable(highfive)

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.0.0
)

# Box2D Build-Optionen konfigurieren
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(box2d)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.71.0 REQUIRED COMPONENTS graph)

# Quellen sammeln
file(GLOB_RECURSE ADVANCED_WARS_SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
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

# Ressourcen kopieren
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/assets)
set(OUTPUT_ASSETS_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets)
file(MAKE_DIRECTORY ${OUTPUT_ASSETS_DIR})
file(GLOB FONT_FILES ${ASSETS_DIR}/*.ttf)

# Executable erstellen
add_executable(advanced_wars ${ADVANCED_WARS_SOURCES})

target_include_directories(advanced_wars
    PRIVATE 
        ${highfive_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)

foreach(FONT ${FONT_FILES})
    add_custom_command(
            TARGET advanced_wars PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${FONT} ${OUTPUT_ASSETS_DIR}
            COMMENT "Kopiere Font: ${FONT} nach ${OUTPUT_ASSETS_DIR}"
    )
endforeach()

set(CMAKE_MODULE_PATH ${ADVANCED_WARS_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

# Plattform-spezifische Konfiguration
if(APPLE)
    # SDL2 Frameworks für macOS
    set(SDL2_PATH "/Library/Frameworks")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    target_include_directories(
            advanced_wars PRIVATE
            ${SDL2_PATH}/SDL2.framework/Headers
            ${SDL2_PATH}/SDL2_image.framework/Headers
            ${SDL2_PATH}/SDL2_ttf.framework/Headers
    )

    target_link_libraries(
            advanced_wars PRIVATE
            ${SDL2_PATH}/SDL2.framework/SDL2
            ${SDL2_PATH}/SDL2_image.framework/SDL2_image
            ${SDL2_PATH}/SDL2_ttf.framework/SDL2_ttf
            Boost::graph  
            box2d
    )

    message(STATUS "Include Dir (SDL2): ${SDL2_PATH}/SDL2.framework/Headers")
    message(STATUS "Include Dir (SDL2_image): ${SDL2_PATH}/SDL2_image.framework/Headers")
    message(STATUS "Include Dir (SDL2_ttf): ${SDL2_PATH}/SDL2_ttf.framework/Headers")
    message(STATUS "Boost Include Dirs: ${Boost_INCLUDE_DIRS}")
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_IMAGE REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(HDF5 REQUIRED COMPONENTS CXX)

    include_directories(/usr/include/SDL2)

    include_directories(${SDL2_INCLUDE_DIR})
    include_directories(${SDL2_IMG_INCLUDE_DIR})
    include_directories(${SDL2_TTF_INCLUDE_DIR})
    include_directories(${HDF5_INCLUDE_DIRS})

    target_link_libraries(advanced_wars
        ${HDF5_LIBRARIES}
        -lSDL2
        -lSDL2_image
        -lSDL2_ttf
        Boost::graph
        box2d
        m
    )
endif()