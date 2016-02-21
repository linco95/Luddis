#include "ScoreGauge.h"
#include "Inventory.h"
#include "ResourceManager.h"

ScoreGauge::ScoreGauge(sf::RenderWindow* aWindow, std::string backgroundFilename, std::string gaugeFilename, sf::Vector2f screenPos) :
mIsActive(true),
mIsAlive(true),
mInventory(&Inventory::getInstance()),
mResourceManager(&ResourceManager::getInstance()),
mFrame(mResourceManager->getTexture(backgroundFilename)),
mGauge(mResourceManager->getTexture(gaugeFilename)),
mGaugeRectSize(mGauge.getTextureRect()){
	setPosition(screenPos);
	setOrigin(0, (float)mGaugeRectSize.height / 2);
	mGauge.move(6, 0); //Offset needed to prevent the image from going outside the frame
}

ScoreGauge::~ScoreGauge(){

}

void ScoreGauge::tick(const sf::Time& deltaTime){
	updateGauge();
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

ScoreGauge::RenderLayer ScoreGauge::getRenderLayer() const{
	return FOREGROUND;
}

void ScoreGauge::updateGauge(){
	if (mInventory->getMaxDust() != 0){
		int maxDust = mInventory->getMaxDust();
		int currentDust = mInventory->getDust();
		int offset = maxDust / 5;
		int gaugeWidth = (int)((float)(mGaugeRectSize.width - (mGaugeRectSize.width * ((float)(currentDust + offset)) / (float)(maxDust + offset))));
		int gaugeHeight = mGaugeRectSize.height;
		int maxX = mGaugeRectSize.width;
		int maxY = mGaugeRectSize.height;
		sf::IntRect gaugeRect(gaugeWidth, 0, maxX, maxY);
		mGauge.setTextureRect(gaugeRect);
	}
}