#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

static const Animation ANIMATION_SWIM = Animation("resources/images/spritesheets/Grafik_silverfisk_SwimSprite_s1d5v2");
static const Animation ANIMATION_HIT = Animation("resources/images/spritesheets/Grafik_silverfisk_DeathSprite_s1d5v2");
static const Animation ANIMATION_DEAD = Animation("resources/images/spritesheets/Grafik_silverfisk_deadSprite_s1d5v2");

static float SPEED = 80;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const int DAMAGE = 10;
static const int LIFE = 15;
static const sf::Vector2f FRONTVECTOR(-1, 0);

static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(55, 17));

Silverfish::Silverfish(sf::RenderWindow* window, const sf::Vector2f& position) :
mIsAlive(true),
mIsActive(true),
mSwimAway(false),
mLife(LIFE),
mWindow(window),
mAnimation(Animation(ANIMATION_SWIM)),
mHitbox(new sf::RectangleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	// Set spawn position
	setPosition(position);


	// Chose direction (towards the left)
	int r2 = rand() % 2;
	sf::Vector2f dir;
	// Diagonally up
	if (r2 == 1){
		dir = { -1, 1 };
		mDirection = VectorMath::normalizeVector(dir);
	}
	// Diagonally down
	else if (r2 == 0){
		dir = { -1, -1 };
		mDirection = VectorMath::normalizeVector(dir);
	}


	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

}

Silverfish::~Silverfish(){
	delete mHitbox;
}

void Silverfish::tick(const sf::Time& deltaTime){
	updateMovement(deltaTime);
	mAnimation.tick(deltaTime);
	if (getPosition().y<(-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) || getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2){
		mIsAlive = false;
	}
}

void Silverfish::updateMovement(const sf::Time& deltaTime){
	float rotation = VectorMath::getAngle(sf::Vector2f(FRONTVECTOR), mDirection) * 180 / (float)M_PI;
	setRotation(rotation);
	move(mDirection * SPEED * deltaTime.asSeconds());
}

void Silverfish::draw(sf::RenderTarget& target, sf::RenderStates states) const{
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
	return ENEMY;
}

Silverfish::Type Silverfish::getCollisionType(){
	return REC;
}

void Silverfish::collide(CollidableEntity *collidable){
	if (collidable->getCollisionCategory() == FRIEND || collidable->getCollisionCategory() == HAIR){
		if (mSwimAway == false){
			mLife -= 5;
			if (mLife <= 0){
				mAnimation.replaceAnimation(ANIMATION_HIT);
				mAnimation.setDefaultAnimation(ANIMATION_DEAD);
				mSwimAway = true;
				mDirection = sf::Vector2f(0, -1);
				SPEED = 120;
			}
		}
	}
}

sf::FloatRect Silverfish::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}
sf::Shape* Silverfish::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}