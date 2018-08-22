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


set(Boost_USE_STATIC_LIB ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system)

# Add external dependencies
add_subdirectory(src/external)

# Source/Header files
set(NOISE_GEN_HEADERS
    include/noise_gen/noise_module.h
    include/noise_gen/parameter.h
    include/noise_gen/module_manager.h
)

set(NOISE_GEN_SOURCES
    src/noise_gen/noise_module.cpp
    src/noise_gen/module_manager.cpp
)

set(UI_HEADERS
    include/ui/user_interface.h
    include/ui/view_controller/module_manager_controller.h
    include/ui/view_controller/parameter_view.h
)

set(UI_SOURCES
    src/ui/user_interface.cpp
    src/ui/view_controller/module_manager_controller.cpp
)

set(MAPGEN_HEADERS
    include/mapgen_application.h
    include/frame_timer.h

    ${NOISE_GEN_HEADERS}
    ${UI_HEADERS}
)

set(MAPGEN_SOURCES
    src/mapgen_application.cpp
    src/frame_timer.cpp

    ${NOISE_GEN_SOURCES}
    ${UI_SOURCES}
)

add_executable(${PROJECT_NAME}
    ${MAPGEN_HEADERS}
    ${MAPGEN_SOURCES}
)

if (MSVC)
    # TODO: Warning as errors. Block by libnoise modulebase header file
    target_compile_options(${PROJECT_NAME} PUBLIC /W4)
endif(MSVC)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    ${LIBNOISE_INCLUDE_DIRS}
    ${MAGNUM_IMGUI_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
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

    # Boost
    ${Boost_LIBRARIES}
)


# source groups
source_group("Header Files/Noise Gen" FILES ${NOISE_GEN_HEADERS})
source_group("Source Files/Noise Gen" FILES ${NOISE_GEN_SOURCES})
source_group("Header Files/UI"        FILES ${UI_HEADERS})
source_group("Source Files/UI"        FILES ${UI_SOURCES})
