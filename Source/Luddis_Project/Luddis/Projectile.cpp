#include "Projectile.h"
#include "ResourceManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

//The max life time should be entered in milliseconds
Projectile::Projectile(std::string textureFilename, sf::Vector2f direction, sf::Vector2f position, float maxLifeTimeMS):
	mIsAlive(true),
	mDirection(direction),
	mLifeTime(maxLifeTimeMS),
	mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	float rotation = std::atan2f(direction.x, -direction.y) * 180 / (float)M_PI;
	mSprite.setRotation(rotation);
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	mSprite.setPosition(position);
}

Projectile::~Projectile(){
	
}

void Projectile::tick(const sf::Time& deltaTime){
	mLifeTime -= deltaTime.asMilliseconds();
	checkLifeTime();
	updateMovement();
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Projectile::isAlive() const{
	return mIsAlive;
}

void Projectile::updateMovement(){
	mSprite.move(mDirection);
}

void Projectile::checkLifeTime(){
	if (mLifeTime<=0){
		mIsAlive = false;
	}
}