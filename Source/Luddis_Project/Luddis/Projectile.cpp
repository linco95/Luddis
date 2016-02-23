#include "Projectile.h"
#include "ResourceManager.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float ROTATIONSPEED = 1.f;
static const float FADINGFACTOR = 0.4f; /// < Fade the factor towards this value according to its lifetime
// Temporary variable for max time, should be saved for all instances and used in fading. Every projectile should know its total lifetime, to be able to calculate how long they've been alive (% of total)
static const sf::Time MAXLIFETIME = sf::seconds(3);

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
#include "Debug.h"
#include <cassert>
void fadeProjectile(sf::Sprite& a_Sprite, const sf::Time& a_TimeLeft){
	// Get the current color of the sprite (mainly the alpha value is of interest here, but we could also tint the projectiles)
	sf::Color curColor = a_Sprite.getColor();
	// Calculate the precent of time left. 1 = just initialized, 0 = deallocated
	float percentOfTimeLeft = a_TimeLeft / MAXLIFETIME;
	assert(percentOfTimePassed >= 0); // We don't want negative values (i.e. we expect that the timeleft is always smaller than or equal the maxlife)

	// TODO: Could we make it fade proportionally to FADINGFACTOR, instead of a_TimeLeft?

	// Clamp the percentOfTimeLeft to the FADINGFACTOR variable
	if (percentOfTimeLeft < FADINGFACTOR)
		percentOfTimeLeft = FADINGFACTOR;

	// Exponentially lowers the opacity
	//curColor.a *= percentOfTimeLeft;
	
	// Linearilly lowers the opacity (255 is max size of an unsigned int with 8 bytes (sf::Uint8))
	curColor.a = 255 * (unsigned)percentOfTimeLeft;

	// This print takes a lot of resources (only use if needed to debug, will slow down the game even in release)
	//Debug::log("Projectile alpha value: " + std::to_string(curColor.a) + ", percentOfTimeLeft: " + std::to_string(percentOfTimeLeft), Debug::INFO);
	
	a_Sprite.setColor(curColor);
}

void Projectile::tick(const sf::Time& deltaTime){
	mLifeTime -= deltaTime.asSeconds();
	checkLifeTime();
	fadeProjectile(mSprite, sf::seconds(mLifeTime));
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


void Projectile::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
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
