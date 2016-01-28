#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"

static const float SPEED = 50;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const int DAMAGE = 10;

Silverfish::Silverfish(std::string textureFilename, sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	// Set spawn position
	mSprite.setPosition((float)size.x, (float)r1);


	// Chose direction (towards the left)
	int r2 = rand()%2;
	sf::Vector2f dir;
	// Diagonally up
	if (r2 == 1){
		dir = { -1, 1 };
		mDirection = VectorMath::normalizeVector(dir);
	}
	// Diagonally down
	else if (r2 == 0){
		dir = { -1, -1 };
		mDirection = VectorMath::normalizeVector(dir);
	}

}

// TODO
// Spawn "dead" silverfish
Silverfish::~Silverfish(){
	
}

void Silverfish::tick(const sf::Time& deltaTime){
	updateMovement(deltaTime);
}

void Silverfish::updateMovement(const sf::Time& deltaTime){
	mSprite.move(mDirection * SPEED * deltaTime.asSeconds());
}

void Silverfish::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Silverfish::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer Silverfish::getRenderLayer() const{
	return LAYER;
}

Silverfish::Category Silverfish::getCollisionCategory(){
	return ENEMY;
}

Silverfish::Type Silverfish::getCollisionType(){
	return REC;
}


void Silverfish::collide(){
	mIsAlive = false;
}

sf::FloatRect Silverfish::getHitBox(){
	return mSprite.getGlobalBounds();
}