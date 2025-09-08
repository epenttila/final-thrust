#include "core/audio_manager.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm>

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    if (initialized_) return true;
    
    // Initialize SDL3_mixer
    if (!MIX_Init()) {
        std::cerr << "Failed to initialize SDL3_mixer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Open default audio device and create mixer
    SDL_AudioSpec spec = { SDL_AUDIO_F32, 2, 48000 };
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!mixer_) {
        std::cerr << "Failed to create mixer: " << SDL_GetError() << std::endl;
        MIX_Quit();
        return false;
    }
    
    // Create music track
    musicTrack_ = MIX_CreateTrack(mixer_);
    if (!musicTrack_) {
        std::cerr << "Failed to create music track: " << SDL_GetError() << std::endl;
        MIX_DestroyMixer(mixer_);
        MIX_Quit();
        return false;
    }
    
    // Tag the music track for easy control
    MIX_TagTrack(musicTrack_, "music");
    
    // Create sound effect tracks
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        soundTracks_[i] = MIX_CreateTrack(mixer_);
        if (soundTracks_[i]) {
            MIX_TagTrack(soundTracks_[i], "sound");
        }
    }
    
    initialized_ = true;
    
    // Set initial volumes
    setMusicVolume(musicVolume_);
    setSoundVolume(soundVolume_);
    
    std::cout << "Audio system initialized successfully" << std::endl;
    return true;
}

void AudioManager::shutdown() {
    if (!initialized_) return;
    
    stopMusic();
    stopAllSounds();
    
    // Untag and destroy tracks
    if (musicTrack_) {
        MIX_UntagTrack(musicTrack_, "music");
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
    }
    
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i]) {
            MIX_UntagTrack(soundTracks_[i], "sound");
            MIX_DestroyTrack(soundTracks_[i]);
            soundTracks_[i] = nullptr;
            trackAudio_[i] = nullptr;
        }
    }
    
    // Destroy mixer
    if (mixer_) {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
    
    MIX_Quit();
    initialized_ = false;
}

void AudioManager::playMusic(MIX_Audio* audio, float volume, int loops) {
    if (!initialized_ || !audio || !musicTrack_) return;
    
    currentMusic_ = audio;
    
    // Stop any currently playing music
    MIX_StopTrack(musicTrack_, 0);
    
    // Set the audio for the track
    MIX_SetTrackAudio(musicTrack_, audio);
    
    // Set volume
    MIX_SetTrackGain(musicTrack_, volume * musicVolume_ * masterVolume_);
    
    // Create properties for looping
    SDL_PropertiesID props = 0;
    if (loops != 0) {
        props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    }
    
    // Play the track
    MIX_PlayTrack(musicTrack_, props);
    
    if (props) {
        SDL_DestroyProperties(props);
    }
}

void AudioManager::stopMusic() {
    if (!initialized_ || !musicTrack_) return;
    MIX_StopTrack(musicTrack_, 0);
    currentMusic_ = nullptr;
}

void AudioManager::pauseMusic() {
    if (!initialized_ || !musicTrack_) return;
    MIX_PauseTrack(musicTrack_);
}

void AudioManager::resumeMusic() {
    if (!initialized_ || !musicTrack_) return;
    MIX_ResumeTrack(musicTrack_);
}

void AudioManager::fadeOutMusic(int ms) {
    if (!initialized_ || !musicTrack_) return;
    // Convert ms to frames for fade out
    Sint64 frames = MIX_TrackMSToFrames(musicTrack_, ms);
    MIX_StopTrack(musicTrack_, frames);
}

void AudioManager::setMusicVolume(float volume) {
    if (!initialized_ || !musicTrack_) return;
    musicVolume_ = std::clamp(volume, 0.0f, 1.0f);
    MIX_SetTrackGain(musicTrack_, musicVolume_ * masterVolume_);
}

bool AudioManager::isMusicPlaying() const {
    if (!initialized_ || !musicTrack_) return false;
    return MIX_TrackPlaying(musicTrack_);
}

void AudioManager::playSound(MIX_Audio* audio, float volume, int loops) {
    if (!initialized_ || !audio) return;
    
    // Find an available track
    MIX_Track* track = nullptr;
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        int idx = (nextSoundTrack_ + i) % MAX_SOUND_TRACKS;
        if (soundTracks_[idx] && !MIX_TrackPlaying(soundTracks_[idx])) {
            track = soundTracks_[idx];
            nextSoundTrack_ = (idx + 1) % MAX_SOUND_TRACKS;
            break;
        }
    }
    
    if (!track) {
        // All tracks busy, use the oldest one
        track = soundTracks_[nextSoundTrack_];
        nextSoundTrack_ = (nextSoundTrack_ + 1) % MAX_SOUND_TRACKS;
    }
    
    if (!track) return;
    
    
    // Stop any sound on this track and clear its audio
    MIX_StopTrack(track, 0);
    MIX_SetTrackAudio(track, nullptr);  // Clear previous audio first
    
    // Set the new audio and track it
    MIX_SetTrackAudio(track, audio);
    
    // Find which track index this is and store the audio reference
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i] == track) {
            trackAudio_[i] = audio;
            break;
        }
    }
    MIX_SetTrackGain(track, volume * soundVolume_ * masterVolume_);
    
    // Create properties for looping
    SDL_PropertiesID props = 0;
    if (loops != 0) {
        props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    }
    
    MIX_PlayTrack(track, props);
    
    if (props) {
        SDL_DestroyProperties(props);
    }
}

void AudioManager::playSoundWithLoopPoint(MIX_Audio* audio, float volume, int loops, int loopStartMs) {
    if (!initialized_ || !audio) return;
    
    // Find an available track
    MIX_Track* track = nullptr;
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        int idx = (nextSoundTrack_ + i) % MAX_SOUND_TRACKS;
        if (soundTracks_[idx] && !MIX_TrackPlaying(soundTracks_[idx])) {
            track = soundTracks_[idx];
            nextSoundTrack_ = (idx + 1) % MAX_SOUND_TRACKS;
            break;
        }
    }
    
    if (!track) {
        // All tracks busy, use the oldest one
        track = soundTracks_[nextSoundTrack_];
        nextSoundTrack_ = (nextSoundTrack_ + 1) % MAX_SOUND_TRACKS;
    }
    
    if (!track) return;
    
    // Stop any sound on this track and clear its audio
    MIX_StopTrack(track, 0);
    MIX_SetTrackAudio(track, nullptr);  // Clear previous audio first
    
    // Set the new audio and track it
    MIX_SetTrackAudio(track, audio);
    
    // Find which track index this is and store the audio reference
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i] == track) {
            trackAudio_[i] = audio;
            break;
        }
    }
    MIX_SetTrackGain(track, volume * soundVolume_ * masterVolume_);
    
    // Create properties for looping with loop start point
    SDL_PropertiesID props = 0;
    if (loops != 0) {
        std::cout << "AudioManager: Setting up looping with loops=" << loops << " and loop start at " << loopStartMs << "ms" << std::endl;
        props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, loopStartMs);
    }
    
    MIX_PlayTrack(track, props);
    
    if (props) {
        SDL_DestroyProperties(props);
    }
}

void AudioManager::stopSound(MIX_Audio* audio) {
    if (!initialized_ || !audio) return;
    
    // Find the track playing this specific audio and stop it
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i] && trackAudio_[i] == audio && MIX_TrackPlaying(soundTracks_[i])) {
            MIX_StopTrack(soundTracks_[i], 0);
            MIX_SetTrackAudio(soundTracks_[i], nullptr);
            trackAudio_[i] = nullptr;
        }
    }
}

void AudioManager::stopAllSounds() {
    if (!initialized_) return;
    
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i]) {
            MIX_StopTrack(soundTracks_[i], 0);
            MIX_SetTrackAudio(soundTracks_[i], nullptr);
            trackAudio_[i] = nullptr;
        }
    }
}

void AudioManager::setSoundVolume(float volume) {
    if (!initialized_) return;
    soundVolume_ = std::clamp(volume, 0.0f, 1.0f);
    
    // Update volume for all sound tracks
    for (int i = 0; i < MAX_SOUND_TRACKS; ++i) {
        if (soundTracks_[i]) {
            MIX_SetTrackGain(soundTracks_[i], soundVolume_ * masterVolume_);
        }
    }
}

void AudioManager::setMasterVolume(float volume) {
    if (!initialized_) return;
    masterVolume_ = std::clamp(volume, 0.0f, 1.0f);
    setMusicVolume(musicVolume_);  // Update music volume
    setSoundVolume(soundVolume_);  // Update sound volume
}