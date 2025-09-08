#include "states/play_state.h"
#include "states/menu_state.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "core/input_manager.h"
#include "core/audio_manager.h"
#include "core/constants.h"
#include "states/state_manager.h"
#include "entities/player.h"
#include "entities/planet.h"
#include "entities/asteroid.h"
#include "utils/utils.h"
#include <string>
#include <cmath>
#include <iostream>



PlayState::PlayState(int level, int maxLevel) 
    : level_(level), maxLevel_(maxLevel) {
    maxAsteroids_ = level + 1;
    maxPlanets_ = level + 1;
}

void PlayState::onEnter(Application* app) {
    app_ = app;
    
    auto* resources = app_->resourceManager();
    auto* audio = app_->audioManager();
    
    // Load resources
    backgroundTexture_ = resources->texture("assets/images/background.png");
    font_ = resources->loadFont(FONT_PATH, 16); // Use larger font size for better pixel rendering
    
    // Play gameplay music
    auto* music = resources->audio("assets/sounds/variables.mp3", audio->mixer());
    if (music)
        audio->playMusic(music, 0.1f);  // 10% volume to match Flash
    
    // Load sound effects
    thrustSound_ = resources->audio("assets/sounds/thrust.mp3", audio->mixer());
    explosionSound_ = resources->audio("assets/sounds/explosion.mp3", audio->mixer());
    
    
    
    // Initialize world with level number as seed
    initWorld(level_);
}

void PlayState::onExit() {
    // Cleanup
}

void PlayState::update(float deltaTime) {
    auto* input = app_->inputManager();
    
    // ESC returns to menu
    if (input->isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
        app_->stateManager()->changeState(std::make_unique<MenuState>(level_));
        return;
    }
    
    if (!levelComplete_ && !levelFailed_) {
        // Apply gravity FIRST (sets base acceleration)
        applyGravity(player_.get(), GRAVITY_COEFFICIENT_PLAYER);
        
        // Apply gravity to asteroids
        for (auto& asteroid : asteroids_)
            applyGravity(asteroid.get(), GRAVITY_COEFFICIENT_ASTEROID);
        
        // Update player input AFTER gravity (adds thrust to gravity)
        player_->handleInput(input, app_->renderer());
        
        // Handle thrust sound - restart every few frames to avoid timing conflicts
        playerAccelerating_ = player_->isAccelerating() && player_->fuel() > 0;
        if (playerAccelerating_) {
            thrustSoundFrameCounter_++;
            if (thrustSoundFrameCounter_ >= 3) {  // Restart every 3 frames (~20 times per second)
                if (thrustSound_) {
                    app_->audioManager()->playSound(thrustSound_, 0.1f, 0);  // Flash-style restart
                    thrustSoundFrameCounter_ = 0;
                }
            }
        } else {
            thrustSoundFrameCounter_ = 0;
        }
        
        // Update all entities
        player_->update(deltaTime);
        homePlanet_->update(deltaTime);
        
        for (auto& planet : planets_)
            planet->update(deltaTime);
        
        for (auto& asteroid : asteroids_)
            asteroid->update(deltaTime);
        
        // Check collisions
        if (isPlayerCollidingWithWorld() && !playerExploding_) {
            // Play explosion sound ONCE
            if (explosionSound_) {
                app_->audioManager()->playSound(explosionSound_, 0.1f);  // 10% volume like Flash
            }
            
            playerExploding_ = true;
            player_->explode();
        }
        
        // Check win condition
        if (homePlanet_->isInside(player_->center(), 0)) {
            levelComplete_ = true;
            transitionTimer_ = 1.0f;
        }
        
        // Check lose condition
        if (player_->isDead()) {
            levelFailed_ = true;
            transitionTimer_ = 2.0f;
        }
    } else {
        // Immediate continue/restart on mouse click (like Flash version)
        if (input->isMouseButtonJustPressed(SDL_BUTTON_LEFT)) {
            if (levelComplete_)
                nextLevel();
            else if (levelFailed_)
                restartLevel();
        }
    }
}

void PlayState::render(Renderer* renderer) {
    // Draw background
    if (backgroundTexture_) {
        for (int y = 0; y < INTERNAL_HEIGHT; y += 240) {
            for (int x = 0; x < INTERNAL_WIDTH; x += 320) {
                renderer->drawTexture(backgroundTexture_, Vector2(static_cast<float>(x), static_cast<float>(y)));
            }
        }
    }
    
    // Draw planets
    homePlanet_->render(renderer);
    for (auto& planet : planets_)
        planet->render(renderer);
    
    // Draw asteroids
    for (auto& asteroid : asteroids_)
        asteroid->render(renderer);
    
    // Draw player
    player_->render(renderer);
    
    // Draw level text - Flash: FlxText(0, 0, 100, "Level " + level)
    if (font_) {
        const std::string levelText = "Level " + std::to_string(level_ + 1);
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font_, levelText.c_str(), 0, color);
        if (surface) {
            auto* texture = renderer->createTextureFromSurface(surface);
            if (texture) {
                renderer->drawTexture(texture, Vector2(0, 0)); // Flash positions at (0,0)
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surface);
        }
    }
    
    // Draw fuel text - Flash: FlxText(0, 0, FlxG.width) with right alignment
    if (font_ && !player_->isDead()) {
        const std::string fuelText = std::to_string(static_cast<int>(player_->fuel())) + "%";
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font_, fuelText.c_str(), 0, color);
        if (surface) {
            auto* texture = renderer->createTextureFromSurface(surface);
            if (texture) {
                const float x = INTERNAL_WIDTH - surface->w; // Right aligned, no margin
                renderer->drawTexture(texture, Vector2(x, 0)); // Flash positions at y=0
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surface);
        }
    }
    
    // Draw level complete/failed text - Flash: centered with totalHeight calculation
    if (levelComplete_ && font_) {
        // Create both surfaces first to calculate total height like Flash
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* mainSurface = TTF_RenderText_Solid(font_, "You survived!", 0, color);
        SDL_Surface* subSurface = TTF_RenderText_Solid(font_, "Click to continue", 0, color);
        
        if (mainSurface && subSurface) {
            // Flash: totalHeight = text.height + margin + subText.height, margin = 0
            const float totalHeight = mainSurface->h + 0 + subSurface->h;
            const float startY = (INTERNAL_HEIGHT - totalHeight) / 2.0f;
            
            // Main text
            auto* texture = renderer->createTextureFromSurface(mainSurface);
            if (texture) {
                const float x = (INTERNAL_WIDTH - mainSurface->w) / 2.0f;
                renderer->drawTexture(texture, Vector2(x, startY));
                SDL_DestroyTexture(texture);
            }
            
            // Sub text
            texture = renderer->createTextureFromSurface(subSurface);
            if (texture) {
                const float x = (INTERNAL_WIDTH - subSurface->w) / 2.0f;
                const float y = startY + mainSurface->h; // No margin
                renderer->drawTexture(texture, Vector2(x, y));
                SDL_DestroyTexture(texture);
            }
        }
        
        if (mainSurface) SDL_DestroySurface(mainSurface);
        if (subSurface) SDL_DestroySurface(subSurface);
    } else if (levelFailed_ && font_) {
        // Create both surfaces first to calculate total height like Flash
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* mainSurface = TTF_RenderText_Solid(font_, "You are dead", 0, color);
        SDL_Surface* subSurface = TTF_RenderText_Solid(font_, "Click to try again", 0, color);
        
        if (mainSurface && subSurface) {
            // Flash: totalHeight = text.height + margin + subText.height, margin = 0
            const float totalHeight = mainSurface->h + 0 + subSurface->h;
            const float startY = (INTERNAL_HEIGHT - totalHeight) / 2.0f;
            
            // Main text
            auto* texture = renderer->createTextureFromSurface(mainSurface);
            if (texture) {
                const float x = (INTERNAL_WIDTH - mainSurface->w) / 2.0f;
                renderer->drawTexture(texture, Vector2(x, startY));
                SDL_DestroyTexture(texture);
            }
            
            // Sub text
            texture = renderer->createTextureFromSurface(subSurface);
            if (texture) {
                const float x = (INTERNAL_WIDTH - subSurface->w) / 2.0f;
                const float y = startY + mainSurface->h; // No margin
                renderer->drawTexture(texture, Vector2(x, y));
                SDL_DestroyTexture(texture);
            }
        }
        
        if (mainSurface) SDL_DestroySurface(mainSurface);
        if (subSurface) SDL_DestroySurface(subSurface);
    }
}

void PlayState::initWorld(int seed) {
    auto* resources = app_->resourceManager();
    
    // Seed RNG with level number for consistent level generation
    utils::setSeed(static_cast<unsigned int>(seed));
    
    // Create player - Flash: x=16, y=(height-playerHeight)/2
    // Flash player is 8x8, positioned at top-left, so center is at 16+4, (240-8)/2+4 = 20, 120
    player_ = std::make_unique<Player>(Vector2(20, 120), resources);
    
    // Create home planet (target) - Flash: random in right zone
    // Flash: Rectangle(width-ZONE_WIDTH, 0, ZONE_WIDTH-home.width, height-home.height)
    // ZONE_WIDTH = 50, so x: 270 to 270+50-homeWidth, y: 0 to 240-homeHeight
    const float homeWidth = 16.0f; // Flash home planet texture size
    const float homeHeight = 16.0f;
    const float homeX = utils::random(270.0f, 270.0f + 50.0f - homeWidth) + homeWidth/2; // Convert to center
    const float homeY = utils::random(0.0f, 240.0f - homeHeight) + homeHeight/2; // Convert to center
    homePlanet_ = std::make_unique<Planet>(
        Vector2(homeX, homeY),
        PlanetType::Home,
        resources
    );
    
    // Generate random planets - Flash uses grid system
    planets_.clear();
    for (int i = 0; i < maxPlanets_; ++i) {
        // Flash: Rectangle(ZONE_WIDTH, 0, width-ZONE_WIDTH-ZONE_WIDTH, height) with grid positioning
        // For now, use middle area excluding zones like Flash
        const float ZONE_WIDTH = 50.0f;
        const float x = utils::random(ZONE_WIDTH, INTERNAL_WIDTH - ZONE_WIDTH);
        const float y = utils::random(0.0f, static_cast<float>(INTERNAL_HEIGHT));
        const auto type = Planet::randomType();
        
        planets_.push_back(std::make_unique<Planet>(Vector2(x, y), type, resources));
    }
    
    // Generate asteroids (matching Flash implementation)
    asteroids_.clear();
    for (int i = 0; i < maxAsteroids_; ++i) {
        // Flash: Rectangle(ZONE_WIDTH, 0, FlxG.width - ZONE_WIDTH, FlxG.height)
        const float ZONE_WIDTH = 50.0f;
        const float x = utils::random(ZONE_WIDTH, static_cast<float>(INTERNAL_WIDTH));
        const float y = utils::random(0.0f, static_cast<float>(INTERNAL_HEIGHT));
        
        // Flash uses random angle and speed 0-10 
        const float angle = utils::random(0.0f, 360.0f) * (M_PI / 180.0f); // Convert to radians
        const float speed = utils::random(0.0f, 10.0f);
        const Vector2 velocity(std::sin(angle) * speed, std::cos(angle) * speed);
        
        asteroids_.push_back(std::make_unique<Asteroid>(Vector2(x, y), velocity, resources));
    }
}

void PlayState::applyGravity(RoundObject* object, float coefficient) {
    Vector2 gravity(0, 0);
    
    // Apply gravity from all planets (based on Flash implementation)
    // NOTE: Flash does NOT apply gravity from home planet, only from regular planets
    for (const auto& planet : planets_) {
        const auto objectPos = object->center();
        const auto planetPos = planet->center();
        const auto r = objectPos - planetPos;
        const float dd = r.lengthSquared();
        
        if (dd > 1.0f) {
            // Flash formula: g = coeff / dd * r
            gravity += r * (coefficient / dd);
        }
    }
    
    // Flash applies negative gravity to acceleration
    object->acceleration_ = gravity * -1.0f;
}

bool PlayState::isPlayerCollidingWithWorld() {
    // Match Flash implementation exactly
    const auto playerCenter = player_->center();
    
    // Check collision with planets - player center inside planet
    for (const auto& planet : planets_) {
        if (planet->isInside(playerCenter, 0))
            return true;
    }
    
    // Check collision with asteroids - player radius overlaps asteroid  
    for (const auto& asteroid : asteroids_) {
        if (asteroid->isInside(playerCenter, player_->radius()))
            return true;
    }
    
    // Check if out of bounds
    const auto pos = player_->position();
    if (pos.x < -50 || pos.x > INTERNAL_WIDTH + 50 ||
        pos.y < -50 || pos.y > INTERNAL_HEIGHT + 50)
        return true;
    
    return false;
}

void PlayState::nextLevel() {
    level_++;
    if (level_ >= maxLevel_) {
        // Game complete - return to menu
        app_->stateManager()->changeState(std::make_unique<MenuState>(0));
    } else {
        // Go to next level
        app_->stateManager()->changeState(std::make_unique<PlayState>(level_, maxLevel_));
    }
}

void PlayState::restartLevel() {
    app_->stateManager()->changeState(std::make_unique<PlayState>(level_, maxLevel_));
}

