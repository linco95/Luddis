#include "Dust.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;

Dust::Dust(std::string textureFilename, sf::RenderWindow* window) :
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

Dust::~Dust(){
	Inventory::getInstance().changeDust(1);
}

void Dust::tick(const sf::Time& deltaTime){
}


void Dust::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Dust::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer Dust::getRenderLayer() const{
	return LAYER;
}

Dust::Category Dust::getCollisionCategory(){
	return COLLECT;
}

Dust::Type Dust::getCollisionType(){
	return REC;
}


void Dust::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		
		SoundEngine::getInstance().playSound("resources/audio/Damm_Slurp_01.wav");
	}
}

sf::FloatRect Dust::getHitBox(){
	return mSprite.getGlobalBounds();
}