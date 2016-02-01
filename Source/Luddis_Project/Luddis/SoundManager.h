#ifndef _INCLUDED_SOUNDMANAGER_
#define _INCLUDED_SOUNDMANAGER_

#include <SFML/Audio.hpp>
#include <string>

class SoundManager{
public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator =(const SoundManager&) = delete;
	~SoundManager();

	//SoundBuffer related functions
	sf::SoundBuffer& getSoundBuffer(std::string filename);
	void loadSoundBuffer(std::string filename);
	void clearSoundBuffer(std::string filename);
	void clearAllSoundBuffers();
protected:
	SoundManager();
private:
	typedef std::vector<std::pair<sf::SoundBuffer*, std::string>> SoundBufferPairVector;
	SoundBufferPairVector mSoundBuffers;
};

#endif // !_INCLUDED_SOUNDMANAGER_
