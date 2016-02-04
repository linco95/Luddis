#include "ScoreCounter.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceManager.h"
#include "Inventory.h"

static const std::string FONT_PATH = "Resources/Fonts/arial.ttf";

ScoreCounter::ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2i screenPos, ScoreType type) :
mAnimation(filename, sf::Vector2i(50, 50), 4, 4, sf::seconds(0.2f)),
mAlive(true),
mWindow(aWindow),
mPosition(screenPos),
mType(type),
mCounter(std::to_string(mScore), ResourceManager::getInstance().getFont(FONT_PATH), 24)
{
	setPosition(mWindow->mapPixelToCoords(mPosition));
	mCounter.setOrigin((float)mCounter.getCharacterSize() / 2, (float)mCounter.getCharacterSize() / 2);
}

ScoreCounter::~ScoreCounter(){

}

void ScoreCounter::tick(const sf::Time& deltaTime){
	mAnimation.tick(deltaTime);
	if (mType == CHIPS){
		mCounter.setString(std::to_string(Inventory::getInstance().getChips()));
	}
	if (mType == DUST){
		mCounter.setString(std::to_string(Inventory::getInstance().getDust()));
	}
	setPosition(mWindow->mapPixelToCoords(mPosition));
}

void ScoreCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation, states);
	target.draw(mCounter, states);
}

bool ScoreCounter::isAlive(){
	return mAlive;
}

ScoreCounter::RenderLayer ScoreCounter::getRenderLayer() const{
	return GUI;
}

void ScoreCounter::setScore(int score){
	mScore = score;
}

void ScoreCounter::addToScore(int score){
	mScore += score;
}