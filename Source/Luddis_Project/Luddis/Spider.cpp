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
static const Renderer::RenderLayer LAYER = Renderer::PLAYER;
static const std::string ANIMATION_ENTER = "resources/images/spritesheets/Spider_enter";
static const std::string ANIMATION_IDLE = "resources/images/spritesheets/Spider_idle";
static const std::string ANIMATION_LEAVE = "resources/images/spritesheets/Spider_climb";

Spider::Spider(sf::RenderWindow* window, const sf::Vector2f& position) :
mIsAlive(true),
mIsActive(true),
mIdleAnimation(false),
mTurn(false),
mLeaving(false),
mWaiting(false),
mWindow(window),
mWeb(sf::Vector2f(2, 0)),
mAnimation(ANIMATION_ENTER),
mDirection(0,1)
{
	setPosition(position);
	sf::Vector2f webPointTop = getTransform().transformPoint(getPosition().x, 0);
	mWeb.setOutlineThickness(1.5f);
	mWeb.setOrigin(mWeb.getSize().x/2, 0);
	mWeb.setFillColor(sf::Color::White);
	mWeb.setOutlineColor(sf::Color::Black);
}

Spider::~Spider(){

}

void Spider::tick(const sf::Time& deltaTime){
	
	if (!mIsActive) return;
	mAnimation.tick(deltaTime);
	updateMovement(deltaTime);
	
	if (getPosition().y<(-mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height) || 
		getPosition().y > mWindow->getView().getSize().y + mAnimation.getCurrAnimation().getSprite().getGlobalBounds().height){
		mIsAlive = false;
	}
}

void Spider::updateMovement(const sf::Time& deltaTime){
	if (mTurn) {
		mTurn = false;
		mLeaving = true;
		mAnimation.setDefaultAnimation(ANIMATION_LEAVE);
		sf::Vector2f dir2{ 0, -1 };
		mDirection = dir2;
	}
	else if (!mLeaving&&getPosition().y >= 300) {
		if (!mIdleAnimation) {
			mWaiting = true;
			mAnimation.setDefaultAnimation(ANIMATION_IDLE);
			mIdleAnimation = true;
		}
	}
	if (!mWaiting) {
		//sf::Vector2f webPointTop = getTransform().transformPoint(getPosition().x, getPosition().y);
		sf::Vector2f webSize(3, getPosition().y);
		mWeb.setSize(webSize);
		mWeb.setPosition(0, -getPosition().y);
		move(mDirection * SPEED * deltaTime.asSeconds());
	}
}

void Spider::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mWeb, states);
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

Renderer::RenderLayer Spider::getRenderLayer() const{
	return Renderer::RenderLayer::PLAYER;
}

void Spider::stun(const sf::Time & deltaTime){

}

void Spider::turn(){
	mTurn = true;
	mWaiting = false;
}