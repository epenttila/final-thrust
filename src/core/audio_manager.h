#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void shutdown();
    
    // Music management
    void playMusic(MIX_Audio* audio, float volume = 1.0f, int loops = -1);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void fadeOutMusic(int ms);
    void setMusicVolume(float volume);
    bool isMusicPlaying() const;
    
    // Sound effects
    void playSound(MIX_Audio* audio, float volume = 1.0f, int loops = 0);
    void playSoundWithLoopPoint(MIX_Audio* audio, float volume, int loops, int loopStartMs);
    void stopSound(MIX_Audio* audio);
    void stopAllSounds();
    void setSoundVolume(float volume);
    
    void setMasterVolume(float volume);
    
    MIX_Mixer* mixer() const { return mixer_; }

private:
    MIX_Mixer* mixer_ = nullptr;
    MIX_Track* musicTrack_ = nullptr;
    
    // Pool of tracks for sound effects
    static constexpr int MAX_SOUND_TRACKS = 16;
    MIX_Track* soundTracks_[MAX_SOUND_TRACKS] = {};
    MIX_Audio* trackAudio_[MAX_SOUND_TRACKS] = {};  // Track which audio is on each track
    int nextSoundTrack_ = 0;
    
    float masterVolume_ = 1.0f;
    float musicVolume_ = 0.5f;  // Default 50% for music
    float soundVolume_ = 1.0f;  // Default 100% for sound effects
    
    bool initialized_ = false;
    MIX_Audio* currentMusic_ = nullptr;
};