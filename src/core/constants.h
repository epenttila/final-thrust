#pragma once

#include <cstdint>



// Game constants
constexpr int32_t INTERNAL_WIDTH = 320;
constexpr int32_t INTERNAL_HEIGHT = 240;
constexpr int32_t DEFAULT_WINDOW_WIDTH = INTERNAL_WIDTH * 2;
constexpr int32_t DEFAULT_WINDOW_HEIGHT = INTERNAL_HEIGHT * 2;

// Gameplay constants
constexpr int32_t MAX_LEVEL = 81;
constexpr float ZONE_WIDTH = 50.0f;
constexpr float ASTEROID_INTERVAL = 0.0f;
constexpr float INITIAL_FUEL = 100.0f;
constexpr float FUEL_CONSUMPTION_RATE = 60.0f;  // 1 fuel per frame at 60 FPS
constexpr float ACCELERATION_FORCE = 90.0f;  // Flash thrust value of -90
constexpr float ROTATION_SPEED = 180.0f;
constexpr float MAX_VELOCITY = 500.0f;  // Increased max speed

// Physics constants
constexpr float GRAVITY_COEFFICIENT_PLAYER = 100.0f;
constexpr float GRAVITY_COEFFICIENT_ASTEROID = 10.0f;

// Asset paths
constexpr const char* FONT_PATH = "assets/fonts/Born2bSportyFS.otf";
constexpr const char* CURSOR_IMAGE = "assets/images/cursor.png";

