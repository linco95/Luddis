#include "Spider.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>
 
static float SPEED = 180;
static const float WAIT_INTERVAL = 1.8f;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;


Spider::Spider(std::string textureFilename, sf::RenderWindow* window) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mWait(WAIT_INTERVAL),
mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);

	mSprite.setPosition(1500,10);
	sf::Vector2f dir;
	dir = { 0, 1 };
	mDirection = VectorMath::normalizeVector(dir);
}

Spider::~Spider(){
}

void Spider::tick(const sf::Time& deltaTime){
	updateMovement(deltaTime);
}

void Spider::updateMovement(const sf::Time& deltaTime){
	if (getPosition().y >= 500){
		mWait -= deltaTime.asSeconds();
		if (mWait <= 0){
		sf::Vector2f dir2;
		dir2 = { 0, -1 };
		mDirection = VectorMath::normalizeVector(dir2);
		move(mDirection * SPEED * deltaTime.asSeconds());
		mWait = WAIT_INTERVAL;
		}
	}
	else
	move(mDirection * SPEED * deltaTime.asSeconds());
}

void Spider::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Spider::isAlive() const{
	return mIsAlive;
}

bool Spider::isActive() const{
	return mIsActive;
}

void Spider::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Spider::getRenderLayer() const{
	return LAYER;
}