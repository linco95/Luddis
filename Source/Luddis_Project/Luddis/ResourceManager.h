#ifndef _INCLUDED_RESOURCEMANAGER_
#define _INCLUDED_RESOURCEMANAGER_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class ResourceManager{	
public:
	static ResourceManager& getInstance();
	~ResourceManager();

	//Texture related functions
	sf::Texture& getTexture(std::string filename);
	void loadTexture(std::string filename/*, sf::Rect = sf::IntRect*/);
	void clearTexture(std::string filename);
	void clearAllTextures();

	//Font related functions
	sf::Font& getFont(std::string filename);
	void loadFont(std::string filename);
	void clearFont(std::string filename);
	void clearAllFonts();

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

private:
	ResourceManager();
	ResourceManager& operator=(ResourceManager& rM);
	typedef std::vector<std::pair<sf::Texture*, std::string>> TexturePairVector;
	TexturePairVector mTextures;
	typedef std::vector<std::pair<sf::Font*, std::string>> FontPairVector;
	FontPairVector mFonts;
	typedef std::vector<std::pair<sf::SoundBuffer*, std::string>> SoundBufferPairVector;
	SoundBufferPairVector mSoundBuffers;
	typedef std::vector<std::pair<sf::Music*, std::string>> MusicPairVector;
	MusicPairVector mMusic;
};

#endif
