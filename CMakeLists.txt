cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

project(mapgen2)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Find Dependencies
find_package(Magnum REQUIRED GL Sdl2Application)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

find_package(imgui REQUIRED)

# Add external dependencies
add_subdirectory(src/external)

# Source/Header files
set(MAPGEN_HEADERS
    include/mapgen_application.h
    include/frame_timer.h
)

set(MAPGEN_SOURCES
    src/mapgen_application.cpp
    src/frame_timer.cpp
)

add_executable(${PROJECT_NAME}
    ${MAPGEN_HEADERS}
    ${MAPGEN_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    ${LIBNOISE_INCLUDE_DIRS}
    ${MAGNUM_IMGUI_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    # Noise libs
    noise
    noiseutils-static

    # Magnum Middleware
    Magnum::Application
    Magnum::GL
    Magnum::Magnum

    # ImGui
    MagnumImGui
    imgui::imgui
)
