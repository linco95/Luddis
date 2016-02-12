#include "ScoreCounter.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceManager.h"
#include "Inventory.h"

static const std::string FONT_PATH = "Resources/Fonts/arial.ttf";


ScoreCounter::ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2f screenPos, ScoreType type) :
mSprite(ResourceManager::getInstance().getTexture(filename)),
mAlive(true),
mIsActive(true),
mWindow(aWindow),
mPosition(screenPos),
mType(type),
mCounter(std::to_string(mScore), ResourceManager::getInstance().getFont(FONT_PATH), 24)
{
	setPosition(mPosition);
	mSprite.setOrigin((float)mSprite.getTextureRect().height / 2, (float)mSprite.getTextureRect().width / 2);
	mCounter.setOrigin(0, mCounter.getGlobalBounds().height);
	mCounter.move(60, 0);
}

ScoreCounter::~ScoreCounter(){

}

void ScoreCounter::tick(const sf::Time& deltaTime){
	if (mType == CHIPS){
		mCounter.setString(" X " + std::to_string(Inventory::getInstance().getChips()));
	}
	if (mType == DUST){
		mCounter.setString(" X " + std::to_string(Inventory::getInstance().getDust()));
	}
}

void ScoreCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mCounter, states);
}

bool ScoreCounter::isAlive() const{
	return mAlive;
}

bool ScoreCounter::isActive() const{
	return mIsActive;
}

void ScoreCounter::setActive(const bool& active){
	mIsActive = active;
}

ScoreCounter::RenderLayer ScoreCounter::getRenderLayer() const{
	return FOREGROUND;
}

void ScoreCounter::setScore(int score){
	mScore = score;
}

void ScoreCounter::addToScore(int score){
	mScore += score;
}