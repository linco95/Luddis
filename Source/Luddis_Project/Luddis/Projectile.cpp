#include "Projectile.h"
#include "ResourceManager.h"

//The max life time should be entered in milliseconds
Projectile::Projectile(std::string textureFilename, sf::Vector2f direction, sf::Time maxLifeTimeMS):
	mIsAlive(true),
	mDirection(direction),
	mLifeTime(),
	mMaxLifeTime(maxLifeTimeMS),
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)){

}

Projectile::~Projectile(){
	
}

void Projectile::tick(const sf::Time& deltaTime){
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
	if (mLifeTime.getElapsedTime() >= mMaxLifeTime){
		mIsAlive = false;
	}
}