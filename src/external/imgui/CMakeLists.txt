# This file is stolen from vcpkg
# https://github.com/Microsoft/vcpkg/blob/master/ports/imgui/CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project(imgui CXX)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(IMGUI_INCLUDES_PUBLIC
    imgui/imgui.h
    imgui/imconfig.h
)

set(IMGUI_INCLUDES_PRIVATE
    imgui/imgui_internal.h
)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
)

set(IMGUI_ADDONS_INCLUDES
    imgui/addons/imguinodegrapheditor/imguinodegrapheditor.h
    imgui/addons/imguitabwindow/imguitabwindow.h
)

set(IMGUI_ADDONS_SOURCES
    imgui/addons/imguinodegrapheditor/imguinodegrapheditor.cpp
    imgui/addons/imguitabwindow/imguitabwindow.cpp
    gradient_editor/imgui_color_gradient.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${IMGUI_INCLUDES_PUBLIC}
    ${IMGUI_INCLUDES_PRIVATE}
    ${IMGUI_SOURCES}

   ${IMGUI_ADDONS_INCLUDES}
   ${IMGUI_ADDONS_SOURCES}
)
add_library(imgui::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/imgui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gradient_editor>
        $<INSTALL_INTERFACE:imgui>
        $<INSTALL_INTERFACE:gradient_editor> 
)

install(TARGETS ${PROJECT_NAME}
    EXPORT IMGUIExport
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(EXPORT IMGUIExport FILE ${PROJECT_NAME}Config.cmake NAMESPACE ${PROJECT_NAME}:: DESTINATION share/${PROJECT_NAME})

if(NOT IMGUI_SKIP_HEADERS)
    install(
        FILES ${IMGUI_INCLUDES_PUBLIC}
        DESTINATION include
    )
endif()
