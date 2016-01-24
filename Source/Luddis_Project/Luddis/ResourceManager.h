#ifndef _INCLUDED_RESOURCEMANAGER_
#define _INCLUDED_RESOURCEMANAGER_

#include "GraphicManager.h"
#include "SoundManager.h"

class ResourceManager: public GraphicManager, public SoundManager{
public:
	static ResourceManager& getInstance();
	~ResourceManager();
private:
	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
};

#endif
