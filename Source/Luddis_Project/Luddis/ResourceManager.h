#ifndef _INCLUDED_RESOURCEMANAGER_
#define _INCLUDED_RESOURCEMANAGER_

#include "GraphicManager.h"
#include "SoundManager.h"
#include <vector>
#include <string>
#include <map>

class ResourceManager: public GraphicManager, public SoundManager{
public:
	static ResourceManager& getInstance();

	//File handling (JSON) related functions
	std::string& getJsonFile(const std::string& filename);
	std::string& loadJsonFile(const std::string& filename);
	void clearJsonFile(const std::string& filename);
	void clearAllJsonFiles();

	//Reading .png maps
	struct PixelInfo {
		PixelInfo(const sf::Color& aColor, sf::Vector2f aPosition) :
			color(aColor),
			position(aPosition) {}
		sf::Color color;
		sf::Vector2f position;
	};
	typedef std::vector<PixelInfo> PixelVector;
	//Returns a vector containing the color and position
	//of each non white pixel in an image.
	PixelVector getMap(const std::string& filename);
	PixelVector readMap(const std::string& filename);
	void clearMap(const std::string& filename);
	void clearAllMaps();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	~ResourceManager();
private:
	ResourceManager();
	typedef std::map<std::string, std::string> TextFileMap;
	TextFileMap mTextFiles;
	typedef std::map<std::string, PixelVector> PixelMap;
	PixelMap mMapFiles;
};

#endif
