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
static const float WAIT_INTERVAL = 3.0f;
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const std::string ANIMATION_ENTER = "resources/images/spritesheets/Grafik_spindel_SpriteEnterv2";
static const std::string ANIMATION_IDLE = "resources/images/spritesheets/Grafik_spindel_SpriteIdle";
static const std::string ANIMATION_LEAVE = "resources/images/spritesheets/Grafik_spindel_SpriteClimbv2";

//TODO: Put in constructor or something to have the spider display
//level sensitive dialogues.
static const std::string SPIDER_DIALOGUE = "Resources/Configs/Dialogue/SpiderDialogue1.json";

Spider::Spider(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget,GameStateLevel* gameStateLevel) :
mIsAlive(true),
mIsActive(false),
mDisplayDialogue(true),
mActivate(activation),
mWindow(window),
mAnimation(ANIMATION_ENTER),
mWait(WAIT_INTERVAL),
mTarget(aTarget),
mGameStateLevel(gameStateLevel)
{
	setPosition(position);
	sf::Vector2f dir;
	dir = { 0, 1 };
	mDirection = VectorMath::normalizeVector(dir);
}

Spider::~Spider(){

}

void Spider::tick(const sf::Time& deltaTime){
	if (mTarget->getPosition().x >= mActivate){
		mIsActive = true;
		if (mDisplayDialogue){
			//TODO: Make the levelstate create dialogues?
			//Would prevent needing all the extra parameters
			//for creating this object.
			mDisplayDialogue = false;
			sf::Vector2f pos((float)ViewUtility::VIEW_WIDTH*0.3f, (float)ViewUtility::VIEW_HEIGHT - 100);
			mGameStateLevel->createDialogue(SPIDER_DIALOGUE, pos);
		}
	}
	if (!mIsActive) return;
	mAnimation.tick(deltaTime);
	updateMovement(deltaTime);
	if (mWait <= 0)
		mWait = WAIT_INTERVAL;
	if (getPosition().y<(-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2) || getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height / 2){
		mIsAlive = false;
	}
}

void Spider::updateMovement(const sf::Time& deltaTime){
	if (getPosition().y >= 500){
			int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
			mAnimation.replaceAnimation(ANIMATION_IDLE);
			mAnimation.getCurrAnimation().setFrame(frame);
			mWait -= deltaTime.asSeconds();
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
	if (!mIsActive) return;
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