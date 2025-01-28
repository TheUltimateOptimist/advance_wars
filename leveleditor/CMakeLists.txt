# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE LEVELEDITOR_SOURCES
        "${PROJECT_SOURCE_DIR}/leveleditor/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/leveleditor/src/*.hpp"
)

# Add executable
add_executable(editor ${LEVELEDITOR_SOURCES})

target_include_directories(editor
    PRIVATE 
        ${highfive_SOURCE_DIR}/include
)
target_link_libraries(editor ${HDF5_LIBRARIES} Qt6::Widgets)