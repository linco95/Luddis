#include "TutorialText.h"
#include <cmath>
#include <SFML/System/Time.hpp>

static sf::IntRect BOX_SIZE(0, 0, 500, 0);
static const int TEXT_SIZE = 24;
static const std::string FONT = "Resources/Fonts/BuxtonSketch.ttf";


TutorialText::TutorialText(sf::RenderWindow* window, sf::Vector2f position, float activationPoint, std::string text, sf::Transformable* aTarget) :
mIsActive(false),
mIsAlive(true),
mActivate(activationPoint),
mWindow(window),
mTarget(aTarget),
mText(BOX_SIZE, text, TEXT_SIZE, true, FONT){
	setPosition(position);
}

TutorialText::~TutorialText(){

}

void TutorialText::tick(const sf::Time& deltaTime){
	if (mTarget->getPosition().x >= mActivate){
		mIsActive = true;
	}
	if(!mIsActive){
		return;
	}
	else{
		updateText(deltaTime);
	}
}

void TutorialText::updateText(const sf::Time& deltaTime){
	mText.animate(deltaTime);
}

void TutorialText::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if (!mIsActive) return;
	states.transform *= getTransform();
	mWindow->draw(mText, states);
}

bool TutorialText::isAlive() const{
	return mIsAlive;
}

TutorialText::RenderLayer TutorialText::getRenderLayer() const{
	return FOREGROUND;
}

bool TutorialText::isActive() const{
	return mIsActive;
}

void TutorialText::setActive(const bool& active){
	mIsActive = active;
}

void TutorialText::stun(const sf::Time& deltatime) {
	return;
}