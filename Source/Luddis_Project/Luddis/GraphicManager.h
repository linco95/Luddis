#ifndef _INCLUDED_GRAPHICMANAGER_
#define _INCLUDED_GRAPHICMANAGER_

#include <SFML/Graphics.hpp>
#include <string>
#include "Animation.h"

class GraphicManager{
public:
	GraphicManager(const GraphicManager&) = delete;
	GraphicManager& operator =(const GraphicManager&) = delete;
	~GraphicManager();

	//Texture related functions
	sf::Texture& getTexture(std::string filename);
	void loadTexture(std::string filename, sf::IntRect& rect);
	void clearTexture(std::string filename);
	void clearAllTextures();

	//Font related functions
	sf::Font& getFont(std::string filename);
	void loadFont(std::string filename);
	void clearFont(std::string filename);
	void clearAllFonts();

	//Animation related functions
	Animation& getAnimation(std::string filename);
	void loadAnimation(std::string filename);
	void clearAnimation(std::string filename);
	void clearAllAnimations();
protected:
	GraphicManager();
private:
	typedef std::vector<std::pair<sf::Texture*, std::string>> TexturePairVector;
	TexturePairVector mTextures;
	typedef std::vector<std::pair<sf::Font*, std::string>> FontPairVector;
	FontPairVector mFonts;
	typedef std::vector<std::pair<Animation*, std::string>> AnimationPairVector;
	AnimationPairVector mAnimations;
};

#endif // !_INCLUDED_GRAPHICMANAGER_
