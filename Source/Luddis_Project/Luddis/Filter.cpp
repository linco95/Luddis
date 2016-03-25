#include "Filter.h"
#include "ResourceManager.h"
#include "ViewUtility.h"
#include <SFML/Graphics/RenderTarget.hpp>

Filter::Filter(float fadeTimer, FilterType filterType, std::string filename) :
	mTimer(0.0f), mMaxTimer(fadeTimer),
	mIsAlive(true),
	mFade(true), mFadeOut(true),
	mHalfway(false), mComplete(false),
	mFilterType(filterType),
	mFadeEffect(ViewUtility::getViewSize().getSize()) {
	mRed = 255;
	mGreen = 255;
	mBlue = 255;
	//mFadeEffect.setOutlineColor(sf::Color::Black);
	if (filterType == FADEBOTH)
		mMaxTimer /= 2;
	else if (filterType == FADEOUT) {
		mTimer = mMaxTimer;
		mFadeOut = false;
	}

	if (filename != "")
		mFadeEffect.setTexture(&ResourceManager::getInstance().getTexture(filename));
	mFadeEffect.setFillColor(sf::Color(mRed, mGreen, mBlue));
}

Filter::~Filter() {

}

void Filter::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mFadeEffect, states);
}

void Filter::tick(const sf::Time & deltaTime) {
	sf::Color fadeColor(mRed, mBlue, mGreen, (unsigned)(mTimer / mMaxTimer * 255.0f));
	if (mFade && mFadeOut) {
		mTimer += deltaTime.asSeconds();
		mTimer = std::min(mTimer, mMaxTimer);
		mFadeEffect.setFillColor(fadeColor);
		if (mTimer >= mMaxTimer) {
			if (mFilterType == FADEIN)
				mFade = false;
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
			mIsAlive = false;
		}
	}
}

Filter::Strata Filter::getRenderLayer() const {
	return THIRD;
}

bool Filter::isAlive() const {
	return mIsAlive;
}

bool Filter::isActive() const {
	return true;
}

void Filter::setActive(const bool & active) {

}

void Filter::kill() {
	mIsAlive = false;
}

bool Filter::getHalfway() {
	if (mHalfway) {
		mHalfway = false;
		return true;
	}
	else
		return false;
}

bool Filter::getComplete() {
	if (mComplete) {
		mComplete = false;
		return true;
	}
	else
		return false;
}