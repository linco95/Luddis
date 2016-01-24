#ifndef _INCLUDED_RESOURCEMANAGER_
#define _INCLUDED_RESOURCEMANAGER_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "GraphicManager.h"
#include "SoundManager.h"

class ResourceManager: public GraphicManager, public SoundManager{
public:
	static ResourceManager& getInstance();
	~ResourceManager();

private:
	ResourceManager();
	ResourceManager(ResourceManager& rM);
	ResourceManager& operator=(ResourceManager& rM);
};

#endif
