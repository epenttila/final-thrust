project(src)

if(MSVC)
  set(WARNING_OPTIONS /W4 /WX)
else()
  set(WARNING_OPTIONS -Wall -Wextra -Wpedantic -Werror)
endif()
 
add_subdirectory(core)
add_subdirectory(game)

if(ANDROID)
    set(TARGET_NAME main)
    add_library(${TARGET_NAME} SHARED main.cpp)
else()
    set(TARGET_NAME main)
    add_executable(${TARGET_NAME} main.cpp)
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})
endif()

target_compile_options(${TARGET_NAME} PRIVATE ${WARNING_OPTIONS})
target_link_libraries(${TARGET_NAME} core game)

if(NOT ANDROID)
    add_custom_target(copy-assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/share
        COMMENT "Copying assets to ${CMAKE_BINARY_DIR}/share"
    )
    add_dependencies(${TARGET_NAME} copy-assets)
endif()

install(TARGETS ${TARGET_NAME})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/ TYPE DATA)
