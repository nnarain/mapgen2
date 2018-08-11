cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}/cmake
)

project(mapgen2)

find_package(Magnum REQUIRED GL Sdl2Application)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add external dependencies
add_subdirectory(src/external)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${LIBNOISE_INCLUDE_DIRS}
)

# libnoise
target_link_libraries(${PROJECT_NAME}
    noise
    noiseutils-static
)

# magnum
target_link_libraries(${PROJECT_NAME}
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
)