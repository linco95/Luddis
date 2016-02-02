#include "Chips.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;

Chips::Chips(std::string textureFilename, sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	int r2 = rand() % (int)size.x + 1;
	// Set spawn position
	mSprite.setPosition((float)r2, (float)r1);

}

Chips::~Chips(){

}

void Chips::tick(const sf::Time& deltaTime){
}


void Chips::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Chips::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer Chips::getRenderLayer() const{
	return LAYER;
}

Chips::Category Chips::getCollisionCategory(){
	return COLLECT;
}

Chips::Type Chips::getCollisionType(){
	return REC;
}


void Chips::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		// TODO
		// Add to chips counter
		SoundEngine::getInstance().playSound("resources/audio/luddis_crumbgather_s1d2v1.wav");
	}
}

sf::FloatRect Chips::getHitBox(){
	return mSprite.getGlobalBounds();
}