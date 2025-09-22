cmake_minimum_required(VERSION 3.28)
project(FinalThrust VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_program(CLANG_FORMAT
    NAMES clang-format clang-format-21
    PATHS "C:/Program Files/LLVM/bin/"
)

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=2020b4fc42dba44817983e06342e682ecfc3d2f484a581f11cc5731fbe4dce8a
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG 2f5bc17ea63a9691ba39718cb701a9c3a13cc3c2
    VERSION 2f5bc17ea63a9691ba39718cb701a9c3a13cc3c2
)

CPMAddPackage(
    NAME SDL3_image
    GITHUB_REPOSITORY libsdl-org/SDL_image
    GIT_TAG e47ff6fa4e9092eec66c1b95118be0fa574c7933
    VERSION e47ff6fa4e9092eec66c1b95118be0fa574c7933
    OPTIONS "SDLIMAGE_AVIF OFF"
)

CPMAddPackage(
    NAME SDL3_ttf
    GITHUB_REPOSITORY libsdl-org/SDL_ttf
    GIT_TAG 7285911aea1df44f6522a8c43025a962493c6c24
    VERSION 7285911aea1df44f6522a8c43025a962493c6c24
)

CPMAddPackage(
    NAME SDL3_mixer
    GITHUB_REPOSITORY libsdl-org/SDL_mixer
    GIT_TAG 40a580fe85c1092c9719b6588739a5e00517d022
    VERSION 40a580fe85c1092c9719b6588739a5e00517d022
    OPTIONS "SDLMIXER_GME OFF"
)

add_subdirectory(src)

if(CLANG_FORMAT)
    message("Found clang-format: ${CLANG_FORMAT}")

	file(GLOB_RECURSE
		SOURCES
		src/*.cpp
		src/*.h
	)

	set(FILE_LIST "")

	foreach(SOURCE ${SOURCES})
		get_filename_component(FILENAME ${SOURCE} ABSOLUTE)
		string(APPEND FILE_LIST "${FILENAME}\n")
	endforeach()

	file(WRITE ${PROJECT_BINARY_DIR}/clang-format-files.txt ${FILE_LIST})

	add_custom_target(
		clang-format
		COMMAND ${CLANG_FORMAT} --dry-run --Werror --files=${PROJECT_BINARY_DIR}/clang-format-files.txt
	)
endif()

set(CPACK_GENERATOR "ZIP")

include(CPack)
