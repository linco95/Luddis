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
sf::Texture& GraphicManager::getTexture(std::string filename){
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		if (mTextures.at(i).second == filename){
			return *mTextures.at(i).first;
		}
	}
	loadTexture(filename, sf::IntRect());
	return getTexture(filename);
}

//Loads an image into memory. !!Cannot store two files with the same filename!!
void GraphicManager::loadTexture(std::string filename, sf::IntRect& rect){
	for (TexturePairVector::size_type i = 0; i < mTextures.size(); i++){
		assert(mTextures.at(i).second != filename);
	}
	sf::Texture* texture = new sf::Texture();
	bool textureLoaded = texture->loadFromFile(filename, rect);
	assert(textureLoaded);	


	std::pair<sf::Texture*, std::string> p1;
	p1.first = texture;
	p1.second = filename;

	p1.first->setSmooth(true);
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

//Returns a font associated with the filename. If there is none, it will create one
sf::Font& GraphicManager::getFont(std::string filename){
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		if (mFonts.at(i).second == filename){
			return *mFonts.at(i).first;
		}
	}
	loadFont(filename);
	return getFont(filename);
}

//Loads a font into memory. !!Cannot store two files with the same filename!!
void GraphicManager::loadFont(std::string filename){
	for (FontPairVector::size_type i = 0; i < mFonts.size(); i++){
		assert(mFonts.at(i).second != filename);
	}

	sf::Font* font = new sf::Font();
	bool isLoaded = font->loadFromFile(filename);
	assert(isLoaded);

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

//Returns an animation associated with the filename. If there is none, it will create one
Animation& GraphicManager::getAnimation(std::string filename){
	for (AnimationPairVector::size_type i = 0; i < mAnimations.size(); i++){
		if (mAnimations.at(i).second == filename){
			return *mAnimations.at(i).first;
		}
	}
	loadFont(filename);
	return getAnimation(filename);
}

//Loads a font into memory. !!Cannot store two files with the same filename!!
void GraphicManager::loadAnimation(std::string filename){
	for (AnimationPairVector::size_type i = 0; i < mAnimations.size(); i++){
		assert(mFonts.at(i).second != filename);
	}

	Animation* animation = new Animation(filename);

	std::pair<Animation*, std::string> p1;
	p1.first = animation;
	p1.second = filename;

	mAnimations.push_back(p1);
}

//Removes the font associated with the filename from memory.
void GraphicManager::clearAnimation(std::string filename){
	for (AnimationPairVector::size_type i = 0; i < mAnimations.size(); i++){
		if (mAnimations.at(i).second == filename){
			delete mAnimations.at(i).first;
			mAnimations.erase(mAnimations.begin() + i);
		}
	}
}

//Removes all currently allocated fonts from memory
void GraphicManager::clearAllFonts(){
	while (!mAnimations.empty()){
		delete mAnimations.back().first;
		mAnimations.pop_back();
	}
}
