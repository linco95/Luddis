#ifndef _INCLUDED_SOUNDENGINE_
#define _INCLUDED_SOUNDENGINE_

static const int MAX_SOUND_CHANNELS = 128;

#include <SFML/Audio.hpp>

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
	void playSound(std::string filename);
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
	std::string mCurrentMusicFile;
	std::string mFadingMusicFile;
	sf::Sound mSoundChannels[MAX_SOUND_CHANNELS];
	sf::Music* mCurrentMusic;
	sf::Music* mFadingMusic;
};

#endif // !_INCLUDED_SOUNDENGINE_
