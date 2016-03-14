#include "BossFinal.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML\Graphics\Shape.hpp>
#include "ResourceManager.h"
#include "Inventory.h"

//Different states depending on how damaged the boss is.
//State 1
static const std::string ANIMATION_IDLE = ("Resources/Images/Spritesheets/Finalboss_idle");
static const std::string ANIMATION_HIT = ("Resources/Images/Spritesheets/Finalboss_hit");
static const std::string ANIMATION_DEATH = ("Resources/Images/Spritesheets/Finalboss_death");
static const std::string ANIMATION_DEAD = ("Resources/Images/Spritesheets/Finalboss_dead");
static const std::string ANIMATION_SHOOT = ("Resources/Images/Spritesheets/Finalboss_shoot");
static const std::string ANIMATION_SHOOT_START = ("Resources/Images/Spritesheets/Finalboss_shootstart");
static const std::string ANIMATION_SHOOT_STOP = ("Resources/Images/Spritesheets/Finalboss_shootstop");
static const std::string ANIMATION_SUCK = ("Resources/Images/Spritesheets/Finalboss_suck");
static const std::string ANIMATION_SUCK_START = ("Resources/Images/Spritesheets/Finalboss_suckstart");
static const std::string ANIMATION_SUCK_STOP = ("Resources/Images/Spritesheets/Finalboss_suckstop");

static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL1 = 10.0f;
static const float ATTACK_INTERVAL2 = 2.0f;
static const float ATTACK_TIME2 = 5.0f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 300;
static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(320, 690));


BossFinal::BossFinal(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager) :
	mIsAlive(true),
	mDead(false),
	mIsActive(false),
	mWindow(window),
	mEntityManager(entityManager),
	mShooting(false),
	mActivate(activation),
	mLife(MAX_LIFE),
	mAttackInterval1(ATTACK_INTERVAL1),
	mAttackInterval2(ATTACK_INTERVAL2),
	mAttackTime2(ATTACK_TIME2),
	mDirection(0, 1.0f),
	mAnimation(Animation(ANIMATION_IDLE)),
	mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
	mTarget(aTarget)
{
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 4, mHitbox->getLocalBounds().height / 2);
	mHitbox->setPosition(getPosition());
	mHitbox->setRotation(-25);
}

BossFinal::~BossFinal() {
	delete mHitbox;
}

void BossFinal::tick(const sf::Time& deltaTime) {
	if (mDead == true) return;
	if (mTarget->getPosition().x >= mActivate) {
		mIsActive = true;
	}
		mAttackInterval1 -= deltaTime.asSeconds();
		mAttackInterval2 -= deltaTime.asSeconds();
		updateMovement(deltaTime);
		mAnimation.tick(deltaTime);

	if (!mIsActive) return;
}

void BossFinal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool BossFinal::isAlive() const {
	return mIsAlive;
}

bool BossFinal::isActive() const {
	return mIsActive;
}

void BossFinal::setActive(const bool& active) {
	mIsActive = active;
}

Renderer::RenderLayer BossFinal::getRenderLayer() const {
	return Renderer::PLAYER;
}

void BossFinal::updateMovement(const sf::Time& deltaTime) {
	if (getPosition().y < 300
		|| getPosition().y > 1860) {
		mDirection = mDirection*-1.0f;
	}
	move(mDirection);
	
	if (mLife >= 75) {
		if (mAttackInterval1 <= 0) {
			mAnimation.setDefaultAnimation(ANIMATION_SHOOT_START);
			mAnimation.replaceAnimation(ANIMATION_SHOOT);
			mAnimation.setDefaultAnimation(ANIMATION_SHOOT_STOP);
			mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			mAttackInterval1 = ATTACK_INTERVAL1;
		}
	}
	
	if (mLife >= 50 && mLife <= 74) {
		if (mAttackInterval2 <= 0) {
			mAnimation.replaceAnimation(ANIMATION_SUCK_START);
			mAnimation.setDefaultAnimation(ANIMATION_SUCK);
			mAttackTime2 -= deltaTime.asSeconds();
			if (mAttackTime2 <= 0){
				mAnimation.replaceAnimation(ANIMATION_SUCK_STOP);
				mAnimation.setDefaultAnimation(ANIMATION_IDLE);
				mAttackInterval2 = ATTACK_INTERVAL2;
			}
		}
	}
}

void BossFinal::attack() {
}

BossFinal::Category BossFinal::getCollisionCategory() {
	return ENEMY_DAMAGE;
}

BossFinal::Type BossFinal::getCollisionType() {
	return REC;
}

void BossFinal::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {
	if (mDead == true) return;
	if (collidable->getCollisionCategory() == PLAYER_PROJECTILE) {
		if (mLife <= 0) {
			mAnimation.replaceAnimation(ANIMATION_DEATH);
			mAnimation.setDefaultAnimation(ANIMATION_DEAD);
			mDead = true;
		}
		else if (mLife > 0) {
			mLife -= 25;
			mAnimation.replaceAnimation(ANIMATION_HIT);
		}
	}
	if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
			Inventory::getInstance().addDust(-500);
	}
}

sf::FloatRect BossFinal::getHitBox() {
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}
sf::Shape* BossFinal::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	return mHitbox;
}

void BossFinal::stun(const sf::Time& deltatime) {
	mTimeStunned = float(deltatime.asSeconds());
}