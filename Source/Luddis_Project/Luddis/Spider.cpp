#include "Spider.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

static float SPEED = 180;
static const float WAIT_INTERVAL = 1.0f;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const Animation ANIMATION_ENTER = Animation("resources/images/spritesheets/Grafik_spindel_SpriteEnterv2");
static const Animation ANIMATION_IDLE = Animation("resources/images/spritesheets/Grafik_spindel_SpriteIdle");
static const Animation ANIMATION_LEAVE = Animation("resources/images/spritesheets/Grafik_spindel_SpriteClimbv2");

Spider::Spider(sf::RenderWindow* window) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mAnimation(Animation(ANIMATION_ENTER)),
mWait(WAIT_INTERVAL)
{
	setPosition(1500,10);
	sf::Vector2f dir;
	dir = { 0, 1 };
	mDirection = VectorMath::normalizeVector(dir);


}

Spider::~Spider(){
}

void Spider::tick(const sf::Time& deltaTime){

	mAnimation.tick(deltaTime);
	updateMovement(deltaTime);
	if (mWait <= 0)
		mWait = WAIT_INTERVAL;
}

void Spider::updateMovement(const sf::Time& deltaTime){
	if (getPosition().y >= 500){
			mWait -= deltaTime.asSeconds();
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			mAnimation.getCurrAnimation().setFrame(frame);
			if (mWait <= 0){
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.setDefaultAnimation(ANIMATION_LEAVE);
				mAnimation.getCurrAnimation().setFrame(frame);
				sf::Vector2f dir2;
				dir2 = { 0, -1 };
				mDirection = VectorMath::normalizeVector(dir2);
				move(mDirection * SPEED * deltaTime.asSeconds());
			}
	}
	else
	move(mDirection * SPEED * deltaTime.asSeconds());
}


void Spider::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool Spider::isAlive() const{
	return mIsAlive;
}

bool Spider::isActive() const{
	return mIsActive;
}

void Spider::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Spider::getRenderLayer() const{
	return LAYER;
}