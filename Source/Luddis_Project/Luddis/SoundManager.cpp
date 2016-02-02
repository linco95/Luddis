#include "SoundManager.h"
#include <cassert>

SoundManager::SoundManager() :
mSoundBuffers(){

}

SoundManager::~SoundManager(){
	clearAllSoundBuffers();
	clearAllMusic();
}

//Returns a soundbuffer associated with the filename. If there is none, it will create one
sf::SoundBuffer& SoundManager::getSoundBuffer(std::string filename){
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		if (mSoundBuffers.at(i).second == filename){
			return *mSoundBuffers.at(i).first;
		}
	}
	loadSoundBuffer(filename);
	return getSoundBuffer(filename);
}

//Loads a soundbuffer into memory. !!Cannot store two files with the same filename!!
void SoundManager::loadSoundBuffer(std::string filename){
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		assert(mSoundBuffers.at(i).second != filename);
	}

	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	bool isLoaded = soundBuffer->loadFromFile(filename);
	assert(isLoaded);

	std::pair<sf::SoundBuffer*, std::string> p1;
	p1.first = soundBuffer;
	p1.second = filename;

	mSoundBuffers.push_back(p1);
}

//Removes the soundbuffer associated with the filename from memory.
void SoundManager::clearSoundBuffer(std::string filename){
	for (SoundBufferPairVector::size_type i = 0; i < mSoundBuffers.size(); i++){
		if (mSoundBuffers.at(i).second == filename){
			delete mSoundBuffers.at(i).first;
			mSoundBuffers.erase(mSoundBuffers.begin() + i);
		}
	}
}

//Removes all currently allocated soundbuffers from memory
void SoundManager::clearAllSoundBuffers(){
	while (!mSoundBuffers.empty()){
		delete mSoundBuffers.back().first;
		mSoundBuffers.pop_back();
	}
}

//Returns a music track associated with the filename. If there is none, it will create one
sf::Music& SoundManager::getMusic(std::string filename){
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		if (mMusic.at(i).second == filename){
			return *mMusic.at(i).first;
		}
	}
	loadMusic(filename);
	return getMusic(filename);

}

//Loads a music track into memory. !!Cannot store two files with the same filename!!
void SoundManager::loadMusic(std::string filename){
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		assert(mMusic.at(i).second != filename);
	}

	sf::Music* music = new sf::Music();
	bool isLoaded = music->openFromFile(filename);
	assert(isLoaded);
	
	std::pair<sf::Music*, std::string> p1;
	p1.first = music;
	p1.second = filename;
	
	mMusic.push_back(p1);
	
}

//Removes the music track associated with the filename from memory.
void SoundManager::clearMusic(std::string filename){
	for (MusicPairVector::size_type i = 0; i < mMusic.size(); i++){
		if (mMusic.at(i).second == filename){
			delete mMusic.at(i).first;
			mMusic.erase(mMusic.begin() + i);
		}
	}
}

//Removes all currently allocated music tracks from memory
void SoundManager::clearAllMusic(){
	while (!mMusic.empty()){
		delete mMusic.back().first;
		mMusic.pop_back();
	}
}