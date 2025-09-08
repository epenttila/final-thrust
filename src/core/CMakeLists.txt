project(core)

set(SOURCES
    Animation.cpp
    AssetManager.cpp
    AudioManager.cpp
    Game.cpp
    InputManager.cpp
    Renderer.cpp
    StateManager.cpp
    
    Utils.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_OPTIONS})
target_link_libraries(${PROJECT_NAME} PUBLIC 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)
