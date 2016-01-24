#include "GraphicManager.h"
#include <cassert>

GraphicManager::GraphicManager() :
mTextures(), mFonts(){

}

GraphicManager::~GraphicManager(){
	clearAllTextures();
	clearAllFonts();
}

//Returns a texture associated with the filename
sf::Texture& GraphicManager::getTexture(std::string filename){
	assert(mTextures.size() > 0);
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		if (mTextures.at(i).second == filename){
			return *mTextures.at(i).first;
		}
	}
	//TODO : Fix 
	//There is no path out if the resource is not found (compiler warning)
}

//Loads an image into memory. !!Cannot store two files with the same filename!!
void GraphicManager::loadTexture(std::string filename){
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

//Removes the image associated with the filename from memory.
void GraphicManager::clearTexture(std::string filename){
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		if (mTextures.at(i).second == filename){
			delete mTextures.at(i).first;
			mTextures.erase(mTextures.begin() + i);
		}
	}
}

//Removes all currently allocated textures from memory
void GraphicManager::clearAllTextures(){
	while (!mTextures.empty()){
		delete mTextures.back().first;
		mTextures.pop_back();
	}
}

//Returns a font associated with the filename
sf::Font& GraphicManager::getFont(std::string filename){
	assert(mFonts.size() > 0);
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		if (mFonts.at(i).second == filename){
			return *mFonts.at(i).first;
		}
	}
	//TODO : Fix 
	//There is no path out if the resource is not found (compiler warning)
}

//Loads a font into memory. !!Cannot store two files with the same filename!!
void GraphicManager::loadFont(std::string filename){
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

//Removes the font associated with the filename from memory.
void GraphicManager::clearFont(std::string filename){
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		if (mFonts.at(i).second == filename){
			delete mFonts.at(i).first;
			mFonts.erase(mFonts.begin() + i);
		}
	}
}

//Removes all currently allocated fonts from memory
void GraphicManager::clearAllFonts(){
	while (!mFonts.empty()){
		delete mFonts.back().first;
		mFonts.pop_back();
	}
}
