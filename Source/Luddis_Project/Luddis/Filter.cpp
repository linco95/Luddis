#include "Filter.h"
#include "ViewUtility.h"
#include <SFML/Graphics/RenderTarget.hpp>

Filter::Filter(float fadeTimer) :
	mTimer(0.0f), mMaxTimer(fadeTimer),
	mIsAlive(true),
	mFade(true), mFadeOut(true),
	mHalfway(false), mComplete(false),
	mFadeEffect(ViewUtility::getViewSize().getSize()) {

	mFadeEffect.setFillColor(sf::Color(0, 0, 0, 0));
	mFadeEffect.setOutlineColor(sf::Color(0, 0, 0, 0));
}

Filter::~Filter() {

}

void Filter::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mFadeEffect, states);
}

void Filter::tick(const sf::Time & deltaTime){
	sf::Color fadeColor(0, 0, 0, (unsigned)(mTimer * 255.0f));
	if (mFade && mFadeOut) {
		mTimer += deltaTime.asSeconds();
		mTimer = std::min(mTimer, mMaxTimer);
		mFadeEffect.setFillColor(fadeColor);
		if (mTimer >= mMaxTimer) {
			//Halfway
			mHalfway = true;
			mFadeOut = false;
		}
	}
	else if (mFade && !mFadeOut) {
		mTimer -= deltaTime.asSeconds();
		mTimer = std::max(mTimer, 0.0f);
		mFadeEffect.setFillColor(fadeColor);
		if (mTimer <= 0.0f) {
			//Complete
			mComplete = true;
			mFade = false;
		}
	}
}

Filter::Strata Filter::getRenderLayer() const{
	return THIRD;
}

bool Filter::isAlive() const{
	return mIsAlive;
}

bool Filter::isActive() const{
	return true;
}

void Filter::setActive(const bool & active){

}

void Filter::kill(){
	mIsAlive = false;
}

bool Filter::getHalfway(){
	if (mHalfway) {
		mHalfway = false;
		return true;
	}
	else
		return false;
}	

bool Filter::getComplete(){
	if (mComplete) {
		mComplete = false;
		return true;
	}
	else
		return false;
}