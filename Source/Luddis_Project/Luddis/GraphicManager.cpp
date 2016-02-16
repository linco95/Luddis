#include "GraphicManager.h"
#include <cassert>

GraphicManager::GraphicManager() :
mTextures(), mFonts(){

}

GraphicManager::~GraphicManager(){
	clearAllTextures();
	clearAllFonts();
}

//Returns a texture associated with the filename. If there is none, it will create one
sf::Texture& GraphicManager::getTexture(const std::string& filename){
	if (mTextures.find(filename) == mTextures.end())
		loadTexture(filename, sf::IntRect());
	return mTextures[filename];
}

//Loads an image into memory. !!Cannot store two files with the same filename!!
sf::Texture& GraphicManager::loadTexture(const std::string& filename, const sf::IntRect& rect){
	if (mTextures.find(filename) == mTextures.end()){
		sf::Texture *texture = &mTextures[filename];
		bool textureLoaded = texture->loadFromFile(filename, rect);
		assert(textureLoaded);
		texture->setSmooth(true);
	}
	return mTextures[filename];
}

//Removes the image associated with the filename from memory.
void GraphicManager::clearTexture(const std::string& filename){
	mTextures.erase(filename);
}

//Removes all currently allocated textures from memory
void GraphicManager::clearAllTextures(){
	mTextures.clear();
}

//Returns a font associated with the filename. If there is none, it will create one
sf::Font& GraphicManager::getFont(const std::string& filename){
	if (mFonts.find(filename) == mFonts.end())
		loadFont(filename);
	return mFonts[filename];
}

//Loads a font into memory. !!Cannot store two files with the same filename!!
sf::Font& GraphicManager::loadFont(const std::string& filename){
	if (mFonts.find(filename) == mFonts.end()){
		sf::Font* font = &mFonts[filename];
		bool fontLoaded = font->loadFromFile(filename);
		assert(fontLoaded);
	}
	return mFonts[filename];
}

//Removes the font associated with the filename from memory.
void GraphicManager::clearFont(const std::string& filename){
	mFonts.erase(filename);
}

//Removes all currently allocated fonts from memory
void GraphicManager::clearAllFonts(){
	mFonts.clear();
}