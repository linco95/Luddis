#include "ResourceManager.h"
#include <cassert>

ResourceManager& ResourceManager::getInstance(){
	static ResourceManager resourceManager;
	return resourceManager;
}

ResourceManager::ResourceManager() :
mTextures(), mFonts(), mSoundBuffers(), mMusic(){

}

ResourceManager::~ResourceManager(){
	clearAllFonts();
	clearAllSoundBuffers();
	clearAllTextures();
}
#pragma warning ( disable : 4715 )

sf::Texture& ResourceManager::getTexture(std::string filename){
	assert(mTextures.size() > 0);
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		if (mTextures.at(i).second == filename){
			return *mTextures.at(i).first;
		}
	}
}
#pragma warning ( default : 4715 )

void ResourceManager::loadTexture(std::string filename){
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		assert(mTextures.at(i).second != filename);
	}
	sf::Texture* texture = new sf::Texture();
	assert(texture->loadFromFile(filename));
		

	std::pair<sf::Texture*, std::string> p1;
	p1.first = texture;
	p1.second = filename;

	mTextures.push_back(p1);
}

void ResourceManager::clearTexture(std::string filename){
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		if (mTextures.at(i).second == filename){
			delete mTextures.at(i).first;
			mTextures.erase(mTextures.begin() + i);
		}
	}
}

void ResourceManager::clearAllTextures(){
	while (!mTextures.empty()){
		delete mTextures.back().first;
		mTextures.pop_back();
	}
}

#pragma warning ( disable : 4715 )

sf::Font& ResourceManager::getFont(std::string filename){
	assert(mFonts.size() > 0);
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		if (mFonts.at(i).second == filename){
			return *mFonts.at(i).first;
		}
	}
}
#pragma warning ( default : 4715 )

void ResourceManager::loadFont(std::string filename){
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		assert(mFonts.at(i).second != filename);
	}

		sf::Font* font = new sf::Font();
		assert(font->loadFromFile(filename));

		std::pair<sf::Font*, std::string> p1;
		p1.first = font;
		p1.second = filename;

		mFonts.push_back(p1);
}

void ResourceManager::clearFont(std::string filename){
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		if (mFonts.at(i).second == filename){
			delete mFonts.at(i).first;
			mFonts.erase(mFonts.begin() + i);
		}
	}
}

void ResourceManager::clearAllFonts(){
	while (!mFonts.empty()){
		delete mFonts.back().first;
		mFonts.pop_back();
	}
}
#pragma warning ( disable : 4715 )
sf::SoundBuffer& ResourceManager::getSoundBuffer(std::string filename){
	assert(mSoundBuffers.size() > 0);
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		if (mSoundBuffers.at(i).second == filename){
			return *mSoundBuffers.at(i).first;
		}
	}
}
#pragma warning ( default : 4715 )

void ResourceManager::loadSoundBuffer(std::string filename){
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		assert(mSoundBuffers.at(i).second == filename);
	}

	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	assert(soundBuffer->loadFromFile(filename));

	std::pair<sf::SoundBuffer*, std::string> p1;
	p1.first = soundBuffer;
	p1.second = filename;

	mSoundBuffers.push_back(p1);
}

void ResourceManager::clearSoundBuffer(std::string filename){
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		if (mSoundBuffers.at(i).second == filename){
			delete mSoundBuffers.at(i).first;
			mSoundBuffers.erase(mSoundBuffers.begin() + i);
		}
	}
}

void ResourceManager::clearAllSoundBuffers(){
	while (!mSoundBuffers.empty()){
		delete mSoundBuffers.back().first;
		mSoundBuffers.pop_back();
	}
}
#pragma warning ( disable : 4715 )

sf::Music& ResourceManager::getMusic(std::string filename){
	assert(mMusic.size() > 0);
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		if (mMusic.at(i).second == filename){
			return *mMusic.at(i).first;
		}
	}
}
#pragma warning ( default : 4715 )

void ResourceManager::loadMusic(std::string filename){
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		assert(mMusic.at(i).second == filename);
	}

	sf::Music* music = new sf::Music();
	assert(music->openFromFile(filename));

	std::pair<sf::Music*, std::string> p1;
	p1.first = music;
	p1.second = filename;

	mMusic.push_back(p1);
}

void ResourceManager::clearMusic(std::string filename){
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		if (mMusic.at(i).second == filename){
			delete mMusic.at(i).first;
			mMusic.erase(mMusic.begin() + i);
		}
	}
}

void ResourceManager::clearAllMusic(){
	while (!mMusic.empty()){
		delete mMusic.back().first;
		mMusic.pop_back();
	}
}