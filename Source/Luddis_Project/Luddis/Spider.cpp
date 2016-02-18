#include "Spider.h"
#include "Dialogue.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "ViewUtility.h"
#include "GameStateLevel.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

static float SPEED = 180;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const std::string ANIMATION_ENTER = "resources/images/spritesheets/Grafik_spindel_SpriteEnterv2";
static const std::string ANIMATION_IDLE = "resources/images/spritesheets/Grafik_spindel_SpriteIdle";
static const std::string ANIMATION_LEAVE = "resources/images/spritesheets/Grafik_spindel_SpriteClimbv2";

//TODO: Put in constructor or something to have the spider display
//level sensitive dialogues.

Spider::Spider(sf::RenderWindow* window, const sf::Vector2f& position) :
mIsAlive(true),
mIsActive(true),
mIdleAnimation(false),
mTurn(false),
mWindow(window),
mAnimation(ANIMATION_ENTER)
{
	setPosition(position);
	sf::Vector2f dir;
	dir = { 0, 1 };
	mDirection = VectorMath::normalizeVector(dir);
}

Spider::~Spider(){

}

void Spider::tick(const sf::Time& deltaTime){
	
	if (!mIsActive) return;
	mAnimation.tick(deltaTime);
	updateMovement(deltaTime);
	
	if (getPosition().y<(-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) || getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2){
		mIsAlive = false;
	}
}

void Spider::updateMovement(const sf::Time& deltaTime){
	if (getPosition().y >= 500){
		if (!mIdleAnimation){
			mWaiting = true;
			mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			mIdleAnimation = true;
		}

		if (mTurn){
			mTurn = false;
			mAnimation.setDefaultAnimation(ANIMATION_LEAVE);
			sf::Vector2f dir2{0, -1};
			mDirection = dir2;
		}
	}
	if (!mWaiting)
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

Spider::RenderLayer Spider::getRenderLayer() const{
	return FOREGROUND;
}

void Spider::turn(){
	mTurn = true;
	mWaiting = false;
}