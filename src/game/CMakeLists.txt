project(game)

set(SOURCES
    Asteroid.cpp
    Button.cpp
    MenuState.cpp
    Planet.cpp
    Player.cpp
    PlayState.cpp
    RoundObject.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_OPTIONS})
target_link_libraries(${PROJECT_NAME} core)
