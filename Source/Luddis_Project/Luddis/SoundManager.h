#ifndef _INCLUDED_SOUNDMANAGER_
#define _INCLUDED_SOUNDMANAGER_

#include <SFML/Audio.hpp>
#include <string>
#include <map>

class SoundManager{
public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator =(const SoundManager&) = delete;
	~SoundManager();
	//SoundBuffer related functions
	sf::SoundBuffer& getSoundBuffer(const std::string& filename);
	sf::SoundBuffer& loadSoundBuffer(const std::string& filename);
	void clearSoundBuffer(const std::string& filename);
	void clearAllSoundBuffers();

	//Music related functions
	sf::Music& getMusic(const std::string& filename);
	sf::Music& loadMusic(const std::string& filename);
	void clearMusic(const std::string& filename);
	void clearAllMusic();
protected:
	SoundManager();
private:
	typedef std::map<std::string, sf::SoundBuffer> SoundBufferMap;
	SoundBufferMap mSoundBuffers;
	typedef std::map<std::string, sf::Music> MusicMap;
	MusicMap mMusic;
};

#endif // !_INCLUDED_SOUNDMANAGER_
