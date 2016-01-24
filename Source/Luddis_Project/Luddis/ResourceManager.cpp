#include "ResourceManager.h"
#include <cassert>

//Used for retrieving the singleton class instance
ResourceManager& ResourceManager::getInstance(){
	static ResourceManager resourceManager;
	return resourceManager;
}

ResourceManager::ResourceManager() :
ResourceManager::GraphicManager(), ResourceManager::SoundManager(){

}

ResourceManager::~ResourceManager(){
	clearAllTextures();
	clearAllFonts();
	clearAllSoundBuffers();
	clearAllMusic();
}
