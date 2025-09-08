#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <memory>
#include <string>

class Renderer;

class ResourceManager {
public:
    ResourceManager(Renderer* renderer);
    ~ResourceManager();
    
    bool initialize();
    void shutdown();
    
    // Texture management
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* texture(const std::string& path);
    void unloadTexture(const std::string& path);
    
    // Font management
    TTF_Font* loadFont(const std::string& path, int size);
    TTF_Font* font(const std::string& key);
    
    // Audio management - New SDL3_mixer API
    MIX_Audio* loadAudio(const std::string& path, MIX_Mixer* mixer);
    MIX_Audio* audio(const std::string& path, MIX_Mixer* mixer);
    
    // Custom cursor
    SDL_Cursor* loadCustomCursor(const std::string& path, int hotX, int hotY, float scale = 1.0f);
    
    void clearAll();

private:
    struct TextureDeleter {
        void operator()(SDL_Texture* tex) const {
            if (tex) SDL_DestroyTexture(tex);
        }
    };
    
    struct FontDeleter {
        void operator()(TTF_Font* font) const {
            if (font) TTF_CloseFont(font);
        }
    };
    
    struct AudioDeleter {
        void operator()(MIX_Audio* audio) const {
            if (audio) MIX_DestroyAudio(audio);
        }
    };
    
    struct CursorDeleter {
        void operator()(SDL_Cursor* cursor) const {
            if (cursor) SDL_DestroyCursor(cursor);
        }
    };
    
    Renderer* renderer_;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, TextureDeleter>> textures_;
    std::unordered_map<std::string, std::unique_ptr<TTF_Font, FontDeleter>> fonts_;
    std::unordered_map<std::string, std::unique_ptr<MIX_Audio, AudioDeleter>> audioFiles_;
    std::unique_ptr<SDL_Cursor, CursorDeleter> customCursor_;
    float cursorScale_ = 1.0f;
};