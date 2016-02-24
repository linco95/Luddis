#include "ResourceManager.h"
#include "Debug.h"
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
		Debug::log("Could not find the file : " + filename + ". Will load it for you.", Debug::ERRORLEVEL::WARNING);
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
	clearAllJsonFiles();
	clearAllMaps();
}

ResourceManager::PixelVector ResourceManager::getMap(const std::string& filename) {
	if (mMapFiles.find(filename) == mMapFiles.end()) {
		Debug::log("Could not find the file : " + filename + ". Will load it for you.", Debug::ERRORLEVEL::WARNING);
		readMap(filename);
	}
	return mMapFiles[filename];
}

ResourceManager::PixelVector ResourceManager::readMap(const std::string& filename) {
	if (mMapFiles.find(filename) == mMapFiles.end()) {
		PixelVector infoVec;
		sf::Image mapImage;
		bool loaded = mapImage.loadFromFile(filename);
		assert(loaded);

		// pixelArray is a c-style array of all pixels in the image
		const sf::Uint8 *pixelArray = mapImage.getPixelsPtr();
		const std::size_t arraySize = mapImage.getSize().x * mapImage.getSize().y * 4;

		// For every pixel in the image
		for (std::size_t i = 0; i < arraySize; i += 4) {
			sf::Color pixel = { *(pixelArray + i), *(pixelArray + i + 1), *(pixelArray + i + 2), *(pixelArray + i + 3) };
			// If the color isnt white, do something
			if (pixel != sf::Color::White) {
				sf::Vector2<size_t> pixPos;
				pixPos.x = (i / 4) % mapImage.getSize().x;
				pixPos.y = i / (4 * mapImage.getSize().x);
				sf::Vector2f floatVec((float)pixPos.x, (float)pixPos.y);
				PixelInfo info(pixel, floatVec);
				infoVec.push_back(info);
			}
		}
		mMapFiles[filename] = infoVec;
	}
	return mMapFiles[filename];
}

void ResourceManager::clearMap(const std::string& filename) {
	mMapFiles.erase(filename);
}

void ResourceManager::clearAllMaps() {
	mMapFiles.clear();
}