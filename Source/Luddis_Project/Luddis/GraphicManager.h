#ifndef _INCLUDED_GRAPHICMANAGER_
#define _INCLUDED_GRAPHICMANAGER_

#include <SFML/Graphics.hpp>
#include <string>

class GraphicManager{
public:
	GraphicManager(const GraphicManager&) = delete;
	GraphicManager& operator =(const GraphicManager&) = delete;
	~GraphicManager();

	//Texture related functions
	sf::Texture& getTexture(std::string filename);
	void loadTexture(std::string filename/*, TODO : add a second parameter for segmented textures*/);
	void clearTexture(std::string filename);
	void clearAllTextures();

	//Font related functions
	sf::Font& getFont(std::string filename);
	void loadFont(std::string filename);
	void clearFont(std::string filename);
	void clearAllFonts();
protected:
	GraphicManager();
private:
	typedef std::vector<std::pair<sf::Texture*, std::string>> TexturePairVector;
	TexturePairVector mTextures;
	typedef std::vector<std::pair<sf::Font*, std::string>> FontPairVector;
	FontPairVector mFonts;
};

#endif // !_INCLUDED_GRAPHICMANAGER_
