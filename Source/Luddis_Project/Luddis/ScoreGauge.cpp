#include "ScoreGauge.h"
#include "Inventory.h"
#include "ResourceManager.h"

ScoreGauge::ScoreGauge(sf::RenderWindow* aWindow, std::string backgroundFilename, std::string gaugeFilename, sf::Vector2f screenPos, bool distort) :
mDistort(distort),
mIsActive(true),
mIsAlive(true),
mInventory(&Inventory::getInstance()),
mResourceManager(&ResourceManager::getInstance()),
mFrame(mResourceManager->getTexture(backgroundFilename)),
mGauge(mResourceManager->getTexture(gaugeFilename)),
mFrameRectSize(mFrame.getTextureRect()){
	setPosition(screenPos);
	setOrigin(0, (float)mFrameRectSize.height / 2);
	mGauge.move(6, 0); //Offset needed to prevent the image from going outside the frame
}

ScoreGauge::~ScoreGauge(){

}

void ScoreGauge::tick(const sf::Time& deltaTime){

}

void ScoreGauge::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mGauge, states);
	target.draw(mFrame, states);
}

bool ScoreGauge::isAlive() const{
	return mIsAlive;
}

bool ScoreGauge::isActive() const{
	return mIsActive;
}

void ScoreGauge::setActive(const bool& active){
	mIsActive = active;
}

ScoreGauge::Strata ScoreGauge::getRenderLayer() const{
	return FOURTH;
}

void ScoreGauge::kill() {
	mIsAlive = false;
}

void ScoreGauge::updateGauge(float fillPercent) {
	if (mInventory->getMaxDust() != 0) {

		float offset = 0;
		if (mDistort)
			offset = fillPercent / 5;

		int gaugeWidth = (int)((float)(mFrameRectSize.width - (mFrameRectSize.width * ((fillPercent + offset)) / (1 + offset))));
		int gaugeHeight = mFrameRectSize.height;
		int maxX = mFrameRectSize.width - 8;
		int maxY = mFrameRectSize.height;
		sf::IntRect gaugeRect(gaugeWidth, 0, maxX, maxY);
		mGauge.setTextureRect(gaugeRect);
	}
}