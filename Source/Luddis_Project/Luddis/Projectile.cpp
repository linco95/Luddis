#include "Projectile.h"
#include "ResourceManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;

//The max life time should be entered in milliseconds
Projectile::Projectile(std::string textureFilename, sf::Vector2f direction, sf::Vector2f position, float maxLifeTimeMS, Collidable::Category collisionCategory):
	mIsAlive(true),
	mDirection(direction),
	mLifeTime(maxLifeTimeMS),
	mCollisionCategory(collisionCategory),
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
	mLifeTime -= deltaTime.asSeconds();
	checkLifeTime();
	updateMovement(deltaTime);
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Projectile::isAlive() {
	return mIsAlive;
}

void Projectile::updateMovement(const sf::Time& deltaTime){
	mSprite.move(mDirection*deltaTime.asSeconds());
}

void Projectile::checkLifeTime(){
	if (mLifeTime<=0){
		mIsAlive = false;
	}
}

Entity::RenderLayer Projectile::getRenderLayer() const{
	return LAYER;
}

Projectile::Category Projectile::getCollisionCategory(){
	return mCollisionCategory;
}

Projectile::Type Projectile::getCollisionType(){
	return REC;
}


void Projectile::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == ENEMY && mCollisionCategory == HAIR){
		mIsAlive = false;
	}
	if (collidable->getCollisionCategory() == HAIR && mCollisionCategory == ENEMY){
		mIsAlive = false;
	}
	if (collidable->getCollisionCategory() == ENEMY_STUN && mCollisionCategory == HAIR){
		mIsAlive = false;
	}
	if (collidable->getCollisionCategory() == FRIEND && mCollisionCategory == ENEMY_STUN){
		mIsAlive = false;
	}

}

sf::FloatRect Projectile::getHitBox(){
	return mSprite.getGlobalBounds();
}

void Projectile::setTexture(std::string filename){
	mSprite.setTexture(ResourceManager::getInstance().getTexture(filename), true);
}