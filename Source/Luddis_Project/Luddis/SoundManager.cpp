#include "SoundManager.h"
#include "Debug.h"
#include <cassert>

SoundManager::SoundManager() :
mSoundBuffers(){

}

SoundManager::~SoundManager(){
	clearAllSoundBuffers();
	clearAllMusic();
}

//Returns a soundbuffer associated with the filename. If there is none, it will create one
sf::SoundBuffer& SoundManager::getSoundBuffer(const std::string& filename){
	if (mSoundBuffers.find(filename) == mSoundBuffers.end()) {
		Debug::log("Could not find the file : " + filename + ". Will load it for you.", Debug::ERRORLEVEL::WARNING);
		loadSoundBuffer(filename);
	}
	return mSoundBuffers[filename];
}

//Loads a soundbuffer into memory. !!Cannot store two files with the same filename!!
sf::SoundBuffer& SoundManager::loadSoundBuffer(const std::string& filename){
	if (mSoundBuffers.find(filename) == mSoundBuffers.end()){
		sf::SoundBuffer *sb = &mSoundBuffers[filename];
		bool sbLoaded = sb->loadFromFile(filename);
		assert(sbLoaded);
	}
	return mSoundBuffers[filename];
}

//Removes the soundbuffer associated with the filename from memory.
void SoundManager::clearSoundBuffer(const std::string& filename){
	mSoundBuffers.erase(filename);
}

//Removes all currently allocated soundbuffers from memory
void SoundManager::clearAllSoundBuffers(){
	mSoundBuffers.clear();
}

//Returns a music track associated with the filename. If there is none, it will create one
sf::Music& SoundManager::getMusic(const std::string& filename){
	if (mMusic.find(filename) == mMusic.end()) {
		Debug::log("Could not find the file : " + filename + ". Will load it for you.", Debug::ERRORLEVEL::WARNING);
		loadMusic(filename);
	}
	return mMusic[filename];
}

//Loads a music track into memory. !!Cannot store two files with the same filename!!
sf::Music& SoundManager::loadMusic(const std::string& filename){
	if (mMusic.find(filename) == mMusic.end()){
		sf::Music* music = &mMusic[filename];
		bool musicLoaded = music->openFromFile(filename);
		assert(musicLoaded);
	}
	return mMusic[filename];
}

//Removes the music track associated with the filename from memory.
void SoundManager::clearMusic(const std::string& filename){
	mMusic.erase(filename);
}

//Removes all currently allocated music tracks from memory
void SoundManager::clearAllMusic(){
	mMusic.clear();
}