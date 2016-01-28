#include "Level.h"
#include "ResourceManager.h"

using namespace sf;

static const char* BGFILEPATH = "resources/images/level1BG.png";
Level::Level(){

}

Level::~Level(){

}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	ResourceManager::getInstance().loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), 978) ));
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));
	mView = aWindow.getView();
	mView.setCenter(mView.getSize() / 2.0f);
	aWindow.setView(mView);
}

void Level::draw(RenderTarget& target, RenderStates states) const {
	target.draw(mBackground);
}
