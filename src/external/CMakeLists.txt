set(PROJECT_SOURCE_DIR_MAPGEN "${PROJECT_SOURCE_DIR}")

###################################################################################################################
# Libnoise                                                                                                      ###
###################################################################################################################
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/libnoise")

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")
set(BUILD_LIBNOISE_UTILS ON CACHE BOOL "Build noise utils")
set(BUILD_LIBNOISE_EXAMPLES OFF CACHE BOOL "Build libnoise examples")

add_subdirectory(libnoise)
if(MSVC)
    target_compile_options(noiseutils PUBLIC /wd4100)
    target_compile_options(noiseutils-static PUBLIC /wd4100)
    target_compile_options(noise PUBLIC /wd4100)
    target_compile_options(noise-static PUBLIC /wd4100)
endif(MSVC)

set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR_MAPGEN}")

set(LIBNOISE_INCLUDE_DIR_INTERNAL ${CMAKE_CURRENT_LIST_DIR}/libnoise)
set(LIBNOISE_INCLUDE_DIRS "${LIBNOISE_INCLUDE_DIR_INTERNAL};${LIBNOISE_INCLUDE_DIR_INTERNAL}/src" PARENT_SCOPE)

###################################################################################################################
# Magnum ImGui                                                                                                  ###
###################################################################################################################

set(BUILD_EXAMPLE OFF CACHE BOOL "Build shared libraries")
add_subdirectory(magnum-imgui)

set(MAGNUM_IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/magnum-imgui/src PARENT_SCOPE)
