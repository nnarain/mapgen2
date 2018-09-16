cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

project(mapgen2)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBNOISE_HAS_MODERN_CPP_CTORS")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Find Dependencies
find_package(Magnum REQUIRED GL Sdl2Application)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)


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

set(OUTPUT_GEN_HEADERS
    include/output_gen/texture_generator.h
)

set(OUTPUT_GEN_SOURCES
    src/output_gen/texture_generator.cpp
)

set(UI_HEADERS
    include/ui/user_interface.h
    include/ui/view_controller/view.h
    include/ui/view_controller/module_manager_controller.h
    include/ui/view_controller/module_preview.h
    include/ui/view_controller/imgui_demo_view.h
    include/ui/view_controller/imgui_metric_view.h
    include/ui/view_controller/tab_renderer.h
    include/ui/view_controller/node_graph_editor.h
    include/ui/view_controller/output_config_tab.h
    include/ui/view_controller/test_view.h
)

set(UI_SOURCES
    src/ui/user_interface.cpp
    src/ui/view_controller/view.cpp
    src/ui/view_controller/module_manager_controller.cpp
    src/ui/view_controller/module_preview.cpp
    src/ui/view_controller/node_graph_editor.cpp
    src/ui/view_controller/output_config_tab.cpp
)

set(DETAIL_HEADERS
    include/detail/gen/source_counter_visitor.h
    include/detail/gen/validation_visitor.h
    include/detail/gen/set_param_visitor.h
    include/detail/view/parameter_view_visitor.h
)

set(DETAIL_SOURCES

)

set(MAPGEN_HEADERS
    include/mapgen_application.h
    include/frame_timer.h

    ${NOISE_GEN_HEADERS}
    ${OUTPUT_GEN_HEADERS}
    ${UI_HEADERS}
    ${DETAIL_HEADERS}
)

set(MAPGEN_SOURCES
    src/mapgen_application.cpp
    src/frame_timer.cpp

    ${NOISE_GEN_SOURCES}
    ${OUTPUT_GEN_SOURCES}
    ${UI_SOURCES}
    ${DETAIL_SOURCES}
)

add_executable(${PROJECT_NAME}
    ${MAPGEN_HEADERS}
    ${MAPGEN_SOURCES}
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /WX /wd4100 /wd4458)
endif(MSVC)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    ${LIBNOISE_INCLUDE_DIRS}
    ${MAGNUM_IMGUI_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    # Noise libs
    noise-static
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
source_group("Header Files/Noise Gen"  FILES ${NOISE_GEN_HEADERS})
source_group("Source Files/Noise Gen"  FILES ${NOISE_GEN_SOURCES})
source_group("Header Files/Output Gen" FILES ${OUTPUT_GEN_HEADERS})
source_group("Source Files/Output Gen" FILES ${OUTPUT_GEN_SOURCES})
source_group("Header Files/UI"         FILES ${UI_HEADERS})
source_group("Source Files/UI"         FILES ${UI_SOURCES})
source_group("Header Files/Detail"     FILES ${DETAIL_HEADERS})
source_group("Source Files/Detail"     FILES ${DETAIL_SOURCES})
