#ifndef _INCLUDED_SOUNDENGINE_
#define _INCLUDED_SOUNDENGINE_

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

static const int MAX_SOUND_CHANNELS = 255;

class SoundEngine{
public:
	SoundEngine(SoundEngine&) = delete;
	SoundEngine& operator=(SoundEngine&) = delete;
	static SoundEngine& getInstance();

	void update(const sf::Time& deltaTime);

	//Volume is a value from 0-100
	void setMainVolume(float volume);
	//Volume is a value from 0-100
	void setSoundVolume(float volume);
	//Volume is a value from 0-100
	void setMusicVolume(float volume);
	//Returns the channel that is playing the sound.
	int playSound(std::string filename);
	void stopSound(int channel);
	void playMusic(std::string filename);
	//Fadetime in seconds
	void fadeToNewMusic(std::string filename, float fadeTime = 1.5f);

private:
	SoundEngine();
	void fadeTransition(const sf::Time& deltaTime);

	float mMainVolume;
	float mSoundVolume;
	float mMusicVolume;
	float mFadeTime;
	float mFadeTimeLeft;
	bool mFading;
	sf::Sound mSoundChannels[MAX_SOUND_CHANNELS];
	sf::Music* mCurrentMusic;
	sf::Music* mFadingMusic;
};

#endif // !_INCLUDED_SOUNDENGINE_
