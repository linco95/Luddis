#include "ResourceManager.h"
#include <fstream>
#include <cassert>

//Used for retrieving the singleton class instance
ResourceManager& ResourceManager::getInstance(){
	static ResourceManager resourceManager;
	return resourceManager;
}

ResourceManager::ResourceManager() :
GraphicManager(), SoundManager(){

}

std::string& ResourceManager::getJsonFile(const std::string& filename){
	if (mTextFiles.find(filename) == mTextFiles.end()){
		loadJsonFile(filename);
	}
	return mTextFiles[filename];
}
std::string& ResourceManager::loadJsonFile(const std::string& filename){
	if (mTextFiles.find(filename) == mTextFiles.end()){
		assert(filename.substr(filename.length() - 5, filename.length()) == ".json");
		std::ifstream input(filename);
		assert(input.is_open());
		std::string inputText = "";
		std::string temp = "";
		while (std::getline(input, temp)){
			inputText += temp + "\n";
		}
		mTextFiles[filename] = inputText;
	}
	return mTextFiles[filename];
}
void ResourceManager::clearJsonFile(const std::string& filename){
	mTextFiles.erase(filename);
}
void ResourceManager::clearAllJsonFiles(){
	mTextFiles.clear();
}


ResourceManager::~ResourceManager(){
	clearAllTextures();
	clearAllFonts();
	clearAllSoundBuffers();
	clearAllMusic();
}
