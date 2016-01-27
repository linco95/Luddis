#include "Level.h"
#include "ResourceManager.h"

using namespace sf;

static const char* BGFILEPATH = "resources/images/level1BG.png";
Level::Level(){

}

Level::~Level(){

}

void Level::initializeLevel(){
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));

}

void Level::draw(RenderTarget& target, RenderStates states) const {
	target.draw(mBackground);
}
