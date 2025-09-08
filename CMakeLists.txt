cmake_minimum_required(VERSION 3.20)
project(FinalThrust VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Store original warning flags
set(ORIGINAL_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(ORIGINAL_C_FLAGS "${CMAKE_C_FLAGS}")

# CPM Configuration
set(CPM_DOWNLOAD_VERSION 0.38.7)
set(CPM_USE_LOCAL_PACKAGES ON)
set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_SOURCE_DIR}/.cpm_cache" CACHE PATH "CPM cache directory")

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake")
    message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}")
    file(DOWNLOAD 
        "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake"
    )
endif()

include("${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake")

# SDL3 dependencies
CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG main
    OPTIONS
        "SDL_SHARED OFF"
        "SDL_STATIC ON"
        "SDL_TEST OFF"
        "SDL_DISABLE_INSTALL ON"
        "SDL_DISABLE_INSTALL_DOCS ON"
)

CPMAddPackage(
    NAME SDL3_image
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG main
    OPTIONS
        "SDL3IMAGE_SHARED OFF"
        "SDL3IMAGE_SAMPLES OFF"
        "SDL3IMAGE_INSTALL OFF"
        "SDL3IMAGE_VENDORED ON"
        "SDL3IMAGE_BACKEND_STB ON"
        "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
    NAME SDL3_ttf
    GITHUB_REPOSITORY libsdl-org/SDL_ttf
    GIT_TAG main
    OPTIONS
        "SDL3TTF_SHARED OFF"
        "SDL3TTF_SAMPLES OFF"
        "SDL3TTF_INSTALL OFF"
        "SDL3TTF_VENDORED ON"
        "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
    NAME SDL3_mixer
    GITHUB_REPOSITORY libsdl-org/SDL_mixer
    GIT_TAG main
    OPTIONS
        "SDL3MIXER_SHARED OFF"
        "SDL3MIXER_SAMPLES OFF"
        "SDL3MIXER_INSTALL OFF"
        "SDL3MIXER_VENDORED ON"
        "BUILD_SHARED_LIBS OFF"
)

# Source files
file(GLOB_RECURSE SOURCES 
    src/*.cpp
)

file(GLOB_RECURSE HEADERS 
    src/*.h
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Enable warnings for our code
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
    SDL3_ttf::SDL3_ttf-static
    SDL3_mixer::SDL3_mixer
)

# Platform-specific settings
if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=3
        -sALLOW_MEMORY_GROWTH=1
        -sASYNCIFY
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets
    )
elseif(ANDROID)
    # Android specific configuration would go here
endif()

# Copy assets to build directory for desktop platforms
if(NOT EMSCRIPTEN AND NOT ANDROID)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()