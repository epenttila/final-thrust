#include "AudioManager.h"
#include <stdexcept>
#include <SDL3_mixer/SDL_mixer.h>

namespace core {

AudioManager::AudioManager(int maxSoundTracks)
	: soundTracks_(maxSoundTracks)
{
	if (!MIX_Init())
		throw std::runtime_error(std::string("Failed to initialize SDL_mixer: ") + SDL_GetError());

	if (mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr); !mixer_)
		throw std::runtime_error(std::string("Failed to create mixer: ") + SDL_GetError());

	if (musicTrack_ = MIX_CreateTrack(mixer_); !musicTrack_)
		throw std::runtime_error(std::string("Failed to create music track: ") + SDL_GetError());

	for (auto& track : soundTracks_)
	{
		if (track = MIX_CreateTrack(mixer_); !track)
			throw std::runtime_error(std::string("Failed to create sound track: ") + SDL_GetError());
	}
}

AudioManager::~AudioManager()
{
	if (musicTrack_)
	{
		MIX_DestroyTrack(musicTrack_);
		musicTrack_ = nullptr;
	}

	for (auto& track : soundTracks_)
	{
		if (track)
		{
			MIX_DestroyTrack(track);
			track = nullptr;
		}
	}

	if (mixer_)
	{
		MIX_DestroyMixer(mixer_);
		mixer_ = nullptr;
	}

	MIX_Quit();
}

void AudioManager::playTrackAudio(MIX_Track* track, MIX_Audio* audio, float volume, int loops, int start, int end)
{
	MIX_StopTrack(track, 0);
	MIX_SetTrackAudio(track, audio);
	MIX_SetTrackGain(track, volume);

	SDL_PropertiesID properties = 0;

	if (properties = SDL_CreateProperties(); !properties)
		throw std::runtime_error(std::string("Failed to create properties: ") + SDL_GetError());

	SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
	SDL_SetNumberProperty(properties, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, start);
	SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER, start);
	SDL_SetNumberProperty(properties, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER, end);

	MIX_PlayTrack(track, properties);

	if (properties)
		SDL_DestroyProperties(properties);
}

void AudioManager::playMusic(MIX_Audio* audio, float volume, int loops)
{
	playTrackAudio(musicTrack_, audio, volume, loops, 0, -1);
}

void AudioManager::stopMusic(int ms)
{
	const auto frames = MIX_TrackMSToFrames(musicTrack_, ms);

	if (frames == -1)
		throw std::runtime_error(std::string("Failed to convert milliseconds to frames: ") + SDL_GetError());

	MIX_StopTrack(musicTrack_, frames);
}

bool AudioManager::isMusicPlaying() const
{
	return MIX_TrackPlaying(musicTrack_);
}

MIX_Track* AudioManager::playSound(MIX_Audio* audio, float volume, int loops, int start, int end)
{
	if (!audio)
		return nullptr;

	MIX_Track* track = nullptr;

	for (std::size_t i = 0; i < soundTracks_.size(); ++i)
	{
		const auto index = (nextSoundTrack_ + i) % soundTracks_.size();

		if (soundTracks_[index] && !MIX_TrackPlaying(soundTracks_[index]))
		{
			track = soundTracks_[index];
			nextSoundTrack_ = (index + 1) % static_cast<int>(soundTracks_.size());
			break;
		}
	}

	if (!track)
	{
		track = soundTracks_[nextSoundTrack_];
		nextSoundTrack_ = (nextSoundTrack_ + 1) % soundTracks_.size();
	}

	if (!track)
		return nullptr;

	playTrackAudio(track, audio, volume, loops, start, end);

	return track;
}

} // namespace core
