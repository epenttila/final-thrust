#include "entities/player.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "core/input_manager.h"
#include "core/constants.h"
#include "utils/utils.h"
#include <algorithm>
#include <cmath>



Player::Player(const Vector2& position, ResourceManager* resources)
    : RoundObject(position, 4.0f), resources_(resources), fuel_(INITIAL_FUEL) {
    texture_ = resources_->texture("assets/images/ship.png");
    exhaustTexture_ = resources_->texture("assets/images/exhaust.png");
    explosionTexture_ = resources_->texture("assets/images/explosion.png");
}

void Player::update(float deltaTime) {
    if (exploding_) {
        explosionTimer_ += deltaTime;
        // 7 frames at 7 FPS = 1.0 second
        if (explosionTimer_ >= 7.0f / 7.0f) { // 1.0 second
            exploding_ = false;
            dead_ = true;  // Set dead only after explosion finishes
        }
        return;  // Don't update physics while exploding
    }
    
    if (dead_) {
        return;  // Don't update if dead
    }
    
    // Apply physics
    velocity_ += acceleration_ * deltaTime;
    
    // Limit velocity
    const auto speed = velocity_.length();
    if (speed > MAX_VELOCITY) {
        velocity_ = velocity_.normalized() * MAX_VELOCITY;
    }
    
    position_ += velocity_ * deltaTime;
    
    // Update animation
    animationTimer_ += deltaTime;
    if (animationTimer_ >= 1.0f / ANIMATION_SPEED) {
        animationTimer_ = 0.0f;
        currentFrame_ = (currentFrame_ + 1) % FRAMES_COUNT;
    }
    
    // Reset acceleration for next frame
    acceleration_ = Vector2(0, 0);
}

void Player::render(Renderer* renderer) {
    // Render exhaust sprite FIRST (behind ship) when accelerating
    if (accelerating_ && !dead_ && exhaustTexture_ && fuel_ > 0) {
        // Flash implementation: exhaust is positioned at -exhaust.width with special origin
        // The exhaust rotates around a point that's (exhaust.width + ship.width/2) from its position
        const float exhaustWidth = 8.0f;
        const float exhaustHeight = 8.0f;
        
        // Calculate exhaust position based on rotation
        const float rad = rotation_ * (M_PI / 180.0f);
        // Position exhaust clearly behind the ship with visible separation
        const float distFromShip = FRAME_WIDTH + exhaustWidth; // Full ship width + full exhaust width = 16 pixels
        
        // Position exhaust behind ship
        Vector2 exhaustPos;
        exhaustPos.x = position_.x - std::cos(rad) * distFromShip;
        exhaustPos.y = position_.y - std::sin(rad) * distFromShip;
        
        // Draw exhaust with proper rotation origin
        renderer->drawTextureEx(exhaustTexture_, exhaustPos, 
                               Vector2(exhaustWidth / 2.0f, exhaustHeight / 2.0f), // origin at center
                               rotation_,
                               Vector2(1.0f, 1.0f)); // scale
    }
    
    if (exploding_ && explosionTexture_) {
        // Render explosion animation (7 frames at 7 FPS like Flash)
        const float animSpeed = 7.0f;
        const int frame = static_cast<int>(explosionTimer_ * animSpeed);
        if (frame < 7) { // Only show first 7 frames
            const float frameWidth = 16.0f;  // Each explosion frame is 16x16
            const float frameHeight = 16.0f;
            SDL_FRect src = { static_cast<float>(frame) * frameWidth, 0, frameWidth, frameHeight };
            SDL_FRect dst = { position_.x - frameWidth/2, position_.y - frameHeight/2, frameWidth, frameHeight };
            renderer->drawTexture(explosionTexture_, &src, &dst, 0);
        }
        // Don't render anything else if explosion finished (frame >= 7)
    } else if (!dead_ && !exploding_ && texture_) {
        // Render ship WITHOUT rotation (always upright)
        SDL_FRect src = { 
            static_cast<float>(currentFrame_ * FRAME_WIDTH), 
            0, 
            static_cast<float>(FRAME_WIDTH), 
            static_cast<float>(FRAME_HEIGHT) 
        };
        SDL_FRect dst = { 
            position_.x - FRAME_WIDTH / 2.0f, 
            position_.y - FRAME_HEIGHT / 2.0f, 
            static_cast<float>(FRAME_WIDTH), 
            static_cast<float>(FRAME_HEIGHT) 
        };
        renderer->drawTexture(texture_, &src, &dst, 0); // No rotation for ship sprite
    }
}

void Player::handleInput(InputManager* input, Renderer* renderer) {
    if (dead_) return;
    
    // Point to mouse cursor (like Flash version)
    auto mousePos = input->mousePosition();
    if (renderer) {
        mousePos = renderer->windowToLogical(mousePos);
        const float dx = mousePos.x - position_.x;
        const float dy = mousePos.y - position_.y;
        rotation_ = std::atan2(dy, dx) * (180.0f / M_PI);
    }
    
    // Acceleration with mouse click
    accelerating_ = input->isMouseButtonPressed(SDL_BUTTON_LEFT);
    
    if (accelerating_ && fuel_ > 0)
        accelerate(1.0f / 60.0f);
}

void Player::accelerate(float deltaTime) {
    if (fuel_ <= 0) return;
    
    // Consume fuel (1 per frame like Flash)
    fuel_ -= FUEL_CONSUMPTION_RATE * deltaTime;
    fuel_ = std::max(0.0f, fuel_);
    
    // Apply thrust in direction of rotation (Flash uses negative thrust)
    const float rad = rotation_ * (M_PI / 180.0f);
    const Vector2 thrust(
        std::cos(rad) * ACCELERATION_FORCE,
        std::sin(rad) * ACCELERATION_FORCE
    );
    
    // Add thrust to existing acceleration (which already has gravity)
    acceleration_ += thrust;
}

void Player::rotate(float direction, float deltaTime) {
    rotation_ += direction * ROTATION_SPEED * deltaTime;
    rotation_ = utils::normalizeAngle(rotation_);
}

void Player::explode() {
    if (!exploding_) {
        exploding_ = true;
        explosionTimer_ = 0.0f;
        velocity_ = Vector2(0, 0);
        // Don't set dead_ = true until explosion finishes
    }
}

void Player::reset() {
    dead_ = false;
    exploding_ = false;
    explosionTimer_ = 0.0f;
    fuel_ = INITIAL_FUEL;
    velocity_ = Vector2(0, 0);
    acceleration_ = Vector2(0, 0);
    rotation_ = 0.0f;
    animationTimer_ = 0.0f;
    currentFrame_ = 0;
    exhaustParticles_.clear();
}


