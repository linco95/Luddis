#include "TutorialText.h"
#include <cmath>
#include <SFML/System/Time.hpp>

static sf::IntRect BOX_SIZE(0, 0, 500, 0);
static const int TEXT_SIZE = 24;
static const std::string FONT = "Resources/Fonts/BuxtonSketch.ttf";


TutorialText::TutorialText(sf::RenderWindow* window, sf::Vector2f position, float activationPoint, std::string text, sf::Transformable* aTarget) :
mIsActive(false),
mIsAlive(true),
mAnimating(true),
mActivate(activationPoint),
mWindow(window),
mTarget(aTarget),
mText(BOX_SIZE, "", TEXT_SIZE, FONT),
mFinalText(text),
MAX_ANIMATE_TIME((float)(text.size()/30.0f)),
mAnimateTime(MAX_ANIMATE_TIME){
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
	if (mAnimateTime > 0){
		mAnimateTime -= deltaTime.asSeconds();
		mAnimateTime = std::max(mAnimateTime, 0.0f);
		std::string tempString;
		int maxStringLength = (int)((MAX_ANIMATE_TIME - mAnimateTime)*30);
		for (int i = 0; i < maxStringLength; i++){
			tempString.push_back(mFinalText[i]);
		}
		mText.setString(tempString);
	}
	else if (mAnimating){
		mText.setString(mFinalText);
		mAnimating = false;
	}
}

void TutorialText::draw(sf::RenderTarget& target, sf::RenderStates states) const{
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