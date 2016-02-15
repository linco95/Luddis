#ifndef _INCLUDED_GRAPHICMANAGER_
#define _INCLUDED_GRAPHICMANAGER_

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Animation.h"

class GraphicManager{
public:
	GraphicManager(const GraphicManager&) = delete;
	GraphicManager& operator =(const GraphicManager&) = delete;
	~GraphicManager();



	//Texture related functions
	sf::Texture& getTexture(const std::string& filename);
	sf::Texture& loadTexture(const std::string& filename, const sf::IntRect& rect = sf::IntRect());
	void clearTexture(const std::string& filename);
	void clearAllTextures();

	//Font related functions
	sf::Font& getFont(const std::string& filename);
	sf::Font& loadFont(const std::string& filename);
	void clearFont(const std::string& filename);
	void clearAllFonts();

protected:
	GraphicManager();
private:
	typedef std::map<std::string, sf::Texture> TextureMap;
	TextureMap mTextures;
	typedef std::map<std::string, sf::Font> FontMap;
	FontMap mFonts;
};

#endif // !_INCLUDED_GRAPHICMANAGER_
