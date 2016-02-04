#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

static float SPEED = 80;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const int DAMAGE = 10;
static const int LIFE = 15;
static const sf::Vector2f FRONTVECTOR(-1, 0);

static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Silverfish::Silverfish(sf::RenderWindow* window, const sf::Vector2f& position, std::string textureFilename) :
mIsAlive(true),
mSwimAway(false),
mLife(LIFE),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	// Set spawn position
	setPosition(position);


	// Chose direction (towards the left)
	int r2 = rand()%2;
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

}

Silverfish::~Silverfish(){
	
}

void Silverfish::tick(const sf::Time& deltaTime){
	updateMovement(deltaTime);
	if (getPosition().y<(-mSprite.getGlobalBounds().height / 2) || getPosition().y > mWindow->getView().getSize().y + mSprite.getGlobalBounds().height / 2){ 
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
	target.draw(mSprite, states);
}

bool Silverfish::isAlive(){
	return mIsAlive;
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

void Silverfish::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == FRIEND || collidable->getCollisionCategory() == HAIR){
		if (mSwimAway== false){
		mLife -= 5;
			if (mLife <= 0){
				mSwimAway = true;
				mDirection = sf::Vector2f(0, -1);
				SPEED = 120;
			}
		}
	}
}

sf::FloatRect Silverfish::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}
sf::Shape* Silverfish::getNarrowHitbox() const{
	return mHitbox;
}