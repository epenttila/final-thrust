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
endif()

target_compile_options(${TARGET_NAME} PRIVATE ${WARNING_OPTIONS})
target_link_libraries(${TARGET_NAME} core game)

if(NOT ANDROID)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
    )
endif()
