cmake_minimum_required(VERSION 3.0.2)

project(mapgen2)

# Add external dependencies
add_subdirectory(src/external)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${LIBNOISE_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    noise
)