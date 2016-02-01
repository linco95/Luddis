#include "Level.h"
#include "ResourceManager.h"
#include <SFML\Audio.hpp>
using namespace sf;


static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
static const char* BGFILEPATH = "resources/images/Grafik_Bana1PrototypBakg_S1D2V4.png";
Level::Level(){

}

Level::~Level(){

}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	ResourceManager::getInstance().loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), (int)aWindow.getView().getSize().y)));
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));
	
	Music *music = &ResourceManager::getInstance().getMusic("resources/audio/musik16.wav");
	music->play();

}

void Level::tick(const sf::Time& deltaTime) {

}
bool Level::isAlive() {
	return true;
}
Entity::RenderLayer Level::getRenderLayer() const {
	return LAYER;
}

void Level::draw(RenderTarget& target, RenderStates states) const {
	target.draw(mBackground);
}
