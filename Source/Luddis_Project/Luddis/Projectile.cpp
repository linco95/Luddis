#include "Projectile.h"
#include "ResourceManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float ROTATIONSPEED = 1.f;
static const float FADINGFACTOR = 0.5f;

//The max life time should be entered in milliseconds
Projectile::Projectile(std::string textureFilename, sf::Vector2f direction, sf::Vector2f position, float maxLifeTimeMS, Projectile::Category collisionCategory):
	mIsAlive(true),
	mIsActive(true),
	mDirection(direction),
	mLifeTime(maxLifeTimeMS),
	mCollisionCategory(collisionCategory),
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setRotation((float)(rand() % 360));
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	mHitbox->setScale(getScale());
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

	setPosition(position);
}

Projectile::~Projectile(){
	delete mHitbox;
}
void fadeProjectile(sf::Sprite& a_Sprite, const sf::Time& a_TimeLeft){
	sf::Color curColor = a_Sprite.getColor();
	curColor.a *= 1; // fadingfactor / timeleft -ish
}

void Projectile::tick(const sf::Time& deltaTime){
	mLifeTime -= deltaTime.asSeconds();
	checkLifeTime();
	updateMovement(deltaTime);
	rotate(ROTATIONSPEED);
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Projectile::isAlive() const{
	return mIsAlive;
}

bool Projectile::isActive() const{
	return mIsActive;
}

void Projectile::setActive(const bool& active){
	mIsActive = active;
}

void Projectile::updateMovement(const sf::Time& deltaTime){
	move(mDirection*deltaTime.asSeconds());
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


void Projectile::collide(CollidableEntity *collidable){
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
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

void Projectile::setTexture(std::string filename){
	mSprite.setTexture(ResourceManager::getInstance().getTexture(filename), true);
}
sf::Shape* Projectile::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}
