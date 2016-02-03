#include "ScoreCounter.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ResourceManager.h"

static const std::string FONT_PATH = "Resources/Fonts/arial.ttf";

ScoreCounter::ScoreCounter(sf::RenderWindow* aWindow, std::string filename, sf::Vector2i screenPos) :
mAnimation(filename, sf::Vector2i(50, 50), 4, 4, sf::seconds(0.2f)),
mAlive(true),
mCounter(std::to_string(mScore), ResourceManager::getInstance().getFont(FONT_PATH), 24),
mWindow(aWindow),
mPosition(screenPos)
{
	setPosition(mWindow->mapPixelToCoords(mPosition));
	mCounter.setOrigin((float)mCounter.getCharacterSize() / 2, (float)mCounter.getCharacterSize() / 2);
}

ScoreCounter::~ScoreCounter(){

}

void ScoreCounter::tick(const sf::Time& deltaTime){
	mAnimation.tick(deltaTime);
	mCounter.setString(std::to_string(mScore));
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