cmake_minimum_required(VERSION 2.8)

project(mapgen2)

# Add external dependencies
add_subdirectory(src/external)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

