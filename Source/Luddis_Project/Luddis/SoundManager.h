#ifndef _INCLUDED_SOUNDMANAGER_
#define _INCLUDED_SOUNDMANAGER_

#include <SFML/Audio.hpp>
#include <string>

class SoundManager{
public:
	~SoundManager();

	//SoundBuffer related functions
	sf::SoundBuffer& getSoundBuffer(std::string filename);
	void loadSoundBuffer(std::string filename);
	void clearSoundBuffer(std::string filename);
	void clearAllSoundBuffers();

	//Music related functions
	sf::Music& getMusic(std::string filename);
	void loadMusic(std::string filename);
	void clearMusic(std::string filename);
	void clearAllMusic();
protected:
	SoundManager();
private:
	SoundManager(SoundManager& sM);
	SoundManager& operator =(SoundManager& sM);
	typedef std::vector<std::pair<sf::SoundBuffer*, std::string>> SoundBufferPairVector;
	SoundBufferPairVector mSoundBuffers;
	typedef std::vector<std::pair<sf::Music*, std::string>> MusicPairVector;
	MusicPairVector mMusic;
};

#endif // !_INCLUDED_SOUNDMANAGER_
