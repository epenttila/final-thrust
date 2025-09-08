#include "states/menu_state.h"
#include "states/play_state.h"
#include "entities/player.h"
#include "entities/planet.h"
#include "entities/asteroid.h"
#include "core/application.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "core/input_manager.h"
#include "core/audio_manager.h"
#include "core/constants.h"
#include "states/state_manager.h"
#include <string>
#include <iostream>
#include <cmath>



MenuState::MenuState(int level) : currentLevel_(level) {}

void MenuState::onEnter(Application* app) {
    app_ = app;
    
    auto* resources = app_->resourceManager();
    auto* audio = app_->audioManager();
    
    // Load resources
    backgroundTexture_ = resources->texture("assets/images/menu.png");
    titleTexture_ = resources->texture("assets/images/title.png");
    font_ = resources->loadFont(FONT_PATH, 16); // Use larger font size for better pixel rendering
    
    // Setup buttons
    const float buttonY = INTERNAL_HEIGHT - 60;
    const float buttonSpacing = 3;
    
    // Play button - get actual texture size
    playButton_.normalTexture = resources->texture("assets/images/button.png");
    playButton_.hoverTexture = resources->texture("assets/images/buttonhover.png");
    
    float playButtonWidth, playButtonHeight;
    SDL_GetTextureSize(playButton_.normalTexture, &playButtonWidth, &playButtonHeight);
    playButton_.rect = { (INTERNAL_WIDTH - playButtonWidth) / 2.0f, buttonY, playButtonWidth, playButtonHeight };
    
    // Level selection buttons - get actual texture sizes
    leftButton_.normalTexture = resources->texture("assets/images/buttonleftoff.png");
    leftButton_.hoverTexture = resources->texture("assets/images/buttonlefton.png");
    
    float leftButtonWidth, leftButtonHeight;
    SDL_GetTextureSize(leftButton_.normalTexture, &leftButtonWidth, &leftButtonHeight);
    leftButton_.rect = { playButton_.rect.x, playButton_.rect.y + playButton_.rect.h + buttonSpacing, leftButtonWidth, leftButtonHeight };
    
    rightButton_.normalTexture = resources->texture("assets/images/buttonrightoff.png");
    rightButton_.hoverTexture = resources->texture("assets/images/buttonrighton.png");
    
    float rightButtonWidth, rightButtonHeight;
    SDL_GetTextureSize(rightButton_.normalTexture, &rightButtonWidth, &rightButtonHeight);
    rightButton_.rect = { playButton_.rect.x + playButton_.rect.w - rightButtonWidth, leftButton_.rect.y, rightButtonWidth, rightButtonHeight };
    
    // Play menu music
    auto* music = resources->audio("assets/sounds/solitude.mp3", audio->mixer());
    if (music)
        audio->playMusic(music, 0.1f);  // 10% volume to match Flash
}

void MenuState::onExit() {
    // Cleanup if needed
}

void MenuState::update(float deltaTime) {
    handleInput();
    updateButtons();
    
    if (playPressed_) {
        playTimer_ -= deltaTime;
        if (playTimer_ <= 0) {
            // Fade out menu music
            app_->audioManager()->fadeOutMusic(500);
            
            app_->stateManager()->changeState(
                std::make_unique<PlayState>(currentLevel_, MAX_LEVEL)
            );
            return;
        }
    }
    
    // ESC to quit from menu
    if (app_->inputManager()->isKeyJustPressed(SDL_SCANCODE_ESCAPE))
        app_->quit();
}

void MenuState::render(Renderer* renderer) {
    // Draw background
    if (backgroundTexture_)
        renderer->drawTexture(backgroundTexture_, Vector2(0, 0));
    
      // Draw title
      if (titleTexture_) {
          // Get actual texture dimensions for proper centering
          float titleWidth, titleHeight;
          SDL_GetTextureSize(titleTexture_, &titleWidth, &titleHeight);
          const float titleX = (INTERNAL_WIDTH - titleWidth) / 2.0f;
          renderer->drawTexture(titleTexture_, Vector2(titleX, 16));
      }
    
    // Draw buttons
    auto drawButton = [renderer](const Button& button) {
        auto* texture = button.hovered ? button.hoverTexture : button.normalTexture;
        if (texture) {
            SDL_FRect dst = button.rect;
            renderer->drawTexture(texture, nullptr, &dst, 0);
        }
    };
    
    drawButton(playButton_);
    drawButton(leftButton_);
    drawButton(rightButton_);
    
    // Draw "PLAY" text on button
    if (font_) {
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font_, "PLAY", 0, color);
        if (surface) {
            auto* texture = renderer->createTextureFromSurface(surface);
            if (texture) {
                const float textX = playButton_.rect.x + (playButton_.rect.w - surface->w) / 2.0f;
                const float textY = playButton_.rect.y + (playButton_.rect.h - surface->h) / 2.0f;
                renderer->drawTexture(texture, Vector2(textX, textY));
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surface);
        }
    }
    
    // Draw level text
    if (font_) {
        const std::string levelText = "LVL " + std::to_string(currentLevel_ + 1);
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font_, levelText.c_str(), 0, color);
        if (surface) {
            auto* texture = renderer->createTextureFromSurface(surface);
            if (texture) {
                const float textX = leftButton_.rect.x + leftButton_.rect.w + 
                                   (rightButton_.rect.x - (leftButton_.rect.x + leftButton_.rect.w) - surface->w) / 2.0f;
                const float textY = leftButton_.rect.y + (leftButton_.rect.h - surface->h) / 2.0f;
                renderer->drawTexture(texture, Vector2(textX, textY));
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surface);
        }
    }
    
    // Draw help text
    if (font_) {
        SDL_Color color = {128, 128, 128, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font_, 
            "Click to accelerate, ESC to quit", 0, color);
        if (surface) {
            auto* texture = renderer->createTextureFromSurface(surface);
            if (texture) {
                const float textX = (INTERNAL_WIDTH - surface->w) / 2.0f;
                const float textY = INTERNAL_HEIGHT - 16;
                renderer->drawTexture(texture, Vector2(textX, textY));
                SDL_DestroyTexture(texture);
            }
            SDL_DestroySurface(surface);
        }
    }
}

void MenuState::handleInput() {
    auto* input = app_->inputManager();
    auto* renderer = app_->renderer();
    const auto windowMousePos = input->mousePosition();
    const auto mousePos = renderer->windowToLogical(windowMousePos);
    
    if (input->isMouseButtonJustPressed(SDL_BUTTON_LEFT)) {
        if (playButton_.contains(mousePos)) {
            startGame();
        } else if (leftButton_.contains(mousePos)) {
            levelDown();
        } else if (rightButton_.contains(mousePos)) {
            levelUp();
        }
    }
}

void MenuState::updateButtons() {
    auto* renderer = app_->renderer();
    const auto windowMousePos = app_->inputManager()->mousePosition();
    const auto mousePos = renderer->windowToLogical(windowMousePos);
    
    playButton_.hovered = playButton_.contains(mousePos);
    leftButton_.hovered = leftButton_.contains(mousePos);
    rightButton_.hovered = rightButton_.contains(mousePos);
}

void MenuState::startGame() {
    if (!playPressed_) {
        playPressed_ = true;
        playTimer_ = 0.5f;
    }
}

void MenuState::levelUp() {
    currentLevel_++;
    if (currentLevel_ >= MAX_LEVEL)
        currentLevel_ = MAX_LEVEL - 1;
}

void MenuState::levelDown() {
    currentLevel_--;
    if (currentLevel_ < 0)
        currentLevel_ = 0;
}
