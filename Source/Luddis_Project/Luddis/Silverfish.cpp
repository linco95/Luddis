#define _USE_MATH_DEFINES
#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#include <cmath>

static const std::string ANIMATION_SWIM = "resources/images/spritesheets/Silverfish_Swim";
static const std::string ANIMATION_HIT = "resources/images/spritesheets/Silverfish_death";
static const std::string ANIMATION_DEAD = "resources/images/spritesheets/Silverfish_dead";

static float SPEED = 80;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const int DAMAGE = 10;
static const int LIFE = 15;
static const sf::Vector2f FRONTVECTOR(-1, 0);

static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(55, 17));

Silverfish::Silverfish(sf::RenderWindow* window, const sf::Vector2f& position, const float& angle, const float& activation, Transformable* aTarget) :
mIsAlive(true),
mIsActive(false),
mSwimAway(false),
mBefriend(false),
mLife(LIFE),
mActivate(activation),
mWindow(window),
mAnimation(ANIMATION_SWIM),
mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
mAlignment(ENEMY),
mTarget(aTarget)
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	// Set spawn position
	setPosition(position);

	sf::Vector2f dir;
	dir = { 1, 0 };
	dir = VectorMath::rotateVector(dir, angle);
	mDirection = VectorMath::normalizeVector(dir);

	// TODO flip if changing direction when swimming aways

	if (mDirection.x > 0){
		scale(sf::Vector2f(1, -1));
	}

	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

}

Silverfish::~Silverfish(){
	delete mHitbox;
}

void Silverfish::tick(const sf::Time& deltaTime){
	if (mTimeStunned <= 0) {
		if (mTarget->getPosition().x >= mActivate) {
			mIsActive = true;
		}
		if (!mIsActive) return;
		if (mBefriend) {
			mAlignment = FRIEND;
			mBefriend = false;
		}
		updateMovement(deltaTime);
		mAnimation.tick(deltaTime);
		// TODO: Cleanup, enable fishes to be outside while spawning
		if (getPosition().y<(
			-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) ||
			getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2
			) {
			mIsAlive = false;
		}
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}
}

void Silverfish::updateMovement(const sf::Time& deltaTime){
	float rotation = VectorMath::getAngle(sf::Vector2f(FRONTVECTOR), mDirection) * 180 / (float)M_PI;
	setRotation(rotation);

	// Make the fishes swim faster when swimming away
	float speed = SPEED;
	if (mSwimAway && mAnimation.getCurrAnimation().hasLooped()) {
		if (mDirection != mNextDir) mDirection = mNextDir;
		speed = 250;
	}
	else if (mSwimAway && !mAnimation.getCurrAnimation().hasLooped()) {
		speed = 0;
	}

	move(mDirection * speed * deltaTime.asSeconds());
}

void Silverfish::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool Silverfish::isAlive() const{
	return mIsAlive;
}

bool Silverfish::isActive() const{
	return mIsActive;
}

void Silverfish::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Silverfish::getRenderLayer() const{
	return LAYER;
}

Silverfish::Category Silverfish::getCollisionCategory(){
	return mAlignment;
}

Silverfish::Type Silverfish::getCollisionType(){
	return REC;
}

void Silverfish::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == HAIR){
		if (mSwimAway== false){
		mLife -= 5;
		if (mLife <= 0){
			mAnimation.replaceAnimation(ANIMATION_HIT);
			mAnimation.setDefaultAnimation(ANIMATION_DEAD);
			mSwimAway = true;
			mNextDir = VectorMath::normalizeVector(getPosition() - collidable->getPosition());
			mBefriend = true;
			mTimeStunned = 0;
			}
		}
	}
}

sf::FloatRect Silverfish::getHitBox(){
	if (mIsActive){
		return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
	}
	else {
		// Not a good way to disable collision as all silverfishes would still collide with each other (if they can collide with an area of 0)
		return sf::FloatRect(-999, -999, 0, 0);
	}
}

sf::Shape* Silverfish::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	
	return mHitbox;
}

void Silverfish::stun(const sf::Time& deltatime) {
	if (mSwimAway == false) {
		mTimeStunned = float(deltatime.asSeconds());
	}
	else {
		return;
	}
}