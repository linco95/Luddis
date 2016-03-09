#define _USE_MATH_DEFINES
#include "Projectile.h"
#include "ResourceManager.h"
#include <cmath>
#include "Debug.h"
#include <cassert>

static const Renderer::RenderLayer LAYER = Renderer::ITEM;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);
static const float ROTATIONSPEED = 1.f;
static const float FADINGFACTOR = 0.2f; /// < Fade the factor towards this value according to its lifetime
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
void fadeProjectile(sf::Sprite& a_Sprite, const sf::Time& a_TimeLeft){
	// Get the current color of the sprite (mainly the alpha value is of interest here, but we could also tint the projectiles)
	sf::Color curColor = a_Sprite.getColor();
	// Calculate the percent of time left. 1 = just initialized, 0 = deallocated
	float percentOfTimeLeft = a_TimeLeft / MAXLIFETIME;

	// TODO: Could we make it fade proportionally to FADINGFACTOR, instead of a_TimeLeft?
	// example: percentOfTimeLeft = (a_TimeLeft - FADINGFACTOR * MAXLIFETIME) / (MAXLIFETIME * FADINGFACTOR)

	// We don't want negative values (i.e. we expect that the timeleft is always smaller than or equal the maxlife)
	assert(percentOfTimeLeft >= 0); 

	// Clamp the percentOfTimeLeft to the FADINGFACTOR variable
	if (percentOfTimeLeft < FADINGFACTOR)
		percentOfTimeLeft = FADINGFACTOR;

	// Exponentially lowers the opacity. "Ränta på ränta"
	//curColor.a *= percentOfTimeLeft;
	
	// MAXVALUE of an Uint8 (~ is a bitwise operator, flips all the bits from 0000 0000 to 1111 1111)
	static const sf::Uint8 MAXVALUE = ~0;
	
	// Linearilly lowers the opacity (255 is max size of an unsigned int with 8 bytes (sf::Uint8))
	curColor.a = sf::Uint8(MAXVALUE * percentOfTimeLeft);


	// This print takes a lot of resources (only use if needed to debug, will slow down the game even in release)
	//Debug::log("Projectile alpha value: " + std::to_string(curColor.a) + ", percentOfTimeLeft: " + std::to_string(percentOfTimeLeft), Debug::INFO);
	
	a_Sprite.setColor(curColor);
}

void Projectile::tick(const sf::Time& deltaTime){
	if (mTimeStunned <= 0) {
		mLifeTime -= deltaTime.asSeconds();
		checkLifeTime();
		fadeProjectile(mSprite, sf::seconds(mLifeTime));
		updateMovement(deltaTime);
		rotate(ROTATIONSPEED);
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}
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
		mLifeTime = 0;
		mIsAlive = false;
	}
}

Renderer::RenderLayer Projectile::getRenderLayer() const{
	return LAYER;
}

Projectile::Category Projectile::getCollisionCategory(){
	return mCollisionCategory;
}

Projectile::Type Projectile::getCollisionType(){
	return REC;
}


void Projectile::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == ENEMY_DAMAGE && mCollisionCategory == PLAYER_PROJECTILE){
		mIsAlive = false;
	}
	else if (collidable->getCollisionCategory() == PLAYER_PROJECTILE && mCollisionCategory == ENEMY_DAMAGE){
		mIsAlive = false;
	}
	else if (collidable->getCollisionCategory() == ENEMY_STUN && mCollisionCategory == PLAYER_PROJECTILE){
		mIsAlive = false;
	}
	else if (collidable->getCollisionCategory() == PLAYER_OBJECT && mCollisionCategory == ENEMY_STUN){
		mIsAlive = false;
	}
	else if (collidable->getCollisionCategory() == SOLID) {
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

void Projectile::stun(const sf::Time& deltatime) {
	if (mCollisionCategory == ENEMY_DAMAGE || mCollisionCategory == ENEMY_STUN) {
		mTimeStunned = float(deltatime.asSeconds());
	}
	else {
		return;
	}
}