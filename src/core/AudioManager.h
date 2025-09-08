#pragma once
#include <vector>

struct MIX_Audio;
struct MIX_Mixer;
struct MIX_Track;

namespace core {

class AudioManager
{
public:
	AudioManager(int maxSoundTracks = 8);
	~AudioManager();
	void playMusic(MIX_Audio* audio, float volume = 1.0f, int loops = -1);
	void stopMusic(int ms = 0);
	bool isMusicPlaying() const;
	MIX_Track* playSound(MIX_Audio* audio, float volume = 1.0f, int loops = 0, int start = 0, int end = -1);

private:
	void playTrackAudio(MIX_Track* track, MIX_Audio* audio, float volume, int loops, int start, int end);

	MIX_Mixer* mixer_ = nullptr;
	MIX_Track* musicTrack_ = nullptr;
	std::vector<MIX_Track*> soundTracks_;
	int nextSoundTrack_ = 0;
};

} // namespace core
