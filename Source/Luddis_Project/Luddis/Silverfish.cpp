#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

static const std::string ANIMATION_SWIM = "resources/images/spritesheets/Grafik_silverfisk_SwimSprite_s1d5v2";
static const std::string ANIMATION_HIT = "resources/images/spritesheets/Grafik_silverfisk_DeathSprite_s1d5v2";
static const std::string ANIMATION_DEAD = "resources/images/spritesheets/Grafik_silverfisk_deadSprite_s1d5v2";

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
	if (mTarget->getPosition().x >= mActivate){
		mIsActive = true;
	}
	if (!mIsActive) return;
	updateMovement(deltaTime);
	mAnimation.tick(deltaTime);
	// TODO: Cleanup, enable fishes to be outside while spawning
	if (getPosition().y<(
		-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) ||
		getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2
		){
		mIsAlive = false;
	}
}

void Silverfish::updateMovement(const sf::Time& deltaTime){
	float rotation = VectorMath::getAngle(sf::Vector2f(FRONTVECTOR), mDirection) * 180 / (float)M_PI;
	setRotation(rotation);
	move(mDirection * SPEED * deltaTime.asSeconds());
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

void Silverfish::collide(CollidableEntity *collidable){
	if (collidable->getCollisionCategory() == HAIR){
		if (mSwimAway== false){
		mLife -= 5;
		if (mLife <= 0){
			mAnimation.replaceAnimation(ANIMATION_HIT);
			mAnimation.setDefaultAnimation(ANIMATION_DEAD);
			mSwimAway = true;
			mDirection = sf::Vector2f(0, -1);
			SPEED = 120;
			mAlignment = FRIEND;
			}
		}
	}
}

sf::FloatRect Silverfish::getHitBox(){
	if (mIsActive){
		return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
	}
	else {
		return sf::FloatRect(-999, -999, 0, 0);
	}
}
sf::Shape* Silverfish::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	return mHitbox;
}