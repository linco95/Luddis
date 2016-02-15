#ifndef _INCLUDED_RESOURCEMANAGER_
#define _INCLUDED_RESOURCEMANAGER_

#include "GraphicManager.h"
#include "SoundManager.h"
#include <string>
#include <map>

class ResourceManager: public GraphicManager, public SoundManager{
public:
	static ResourceManager& getInstance();

	//File handeling (JSON) related functions
	std::string& getJsonFile(const std::string& filename);
	std::string& loadJsonFile(const std::string& filename);
	void clearJsonFile(const std::string& filename);
	void clearAllJsonFiles();

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	~ResourceManager();
private:
	ResourceManager();
	typedef std::map<std::string, std::string> TextFileMap;
	TextFileMap mTextFiles;
};

#endif
