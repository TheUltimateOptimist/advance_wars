cmake_minimum_required(VERSION 3.15)

# Projektname und Sprache
project(ADVANCED_WARS
        VERSION 1.0.0
        DESCRIPTION "HSFD C++ Project"
        LANGUAGES CXX
)

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
file(GLOB FONT_FILES ${ASSETS_DIR}/*.TTF)
file(GLOB IMAGE_FILES ${ASSETS_DIR}/*.png)

# Executable erstellen
add_executable(advanced_wars ${ADVANCED_WARS_SOURCES})

foreach(FONT ${FONT_FILES})
    add_custom_command(
            TARGET advanced_wars PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${FONT} ${OUTPUT_ASSETS_DIR}
            COMMENT "Kopiere Font: ${FONT} nach ${OUTPUT_ASSETS_DIR}"
    )
endforeach()

foreach(IMAGE ${IMAGE_FILES})
    add_custom_command(
            TARGET advanced_wars PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${IMAGE} ${OUTPUT_ASSETS_DIR}
            COMMENT "Kopiere Image: ${IMAGE} nach ${OUTPUT_ASSETS_DIR}"
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
    )

    # Debug-Ausgaben
    message(STATUS "Include Dir (SDL2): ${SDL2_PATH}/SDL2.framework/Headers")
    message(STATUS "Include Dir (SDL2_image): ${SDL2_PATH}/SDL2_image.framework/Headers")
    message(STATUS "Include Dir (SDL2_ttf): ${SDL2_PATH}/SDL2_ttf.framework/Headers")
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
        m
    )


endif()

