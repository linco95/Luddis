#include "GameStateLevel.h"

GameStateLevel::GameStateLevel():
mEM(&EntityManager::getInstance()),
mGUIM(&GUIManager::getInstance()){

}

GameStateLevel::~GameStateLevel(){

}

void GameStateLevel::update(sf::Clock& clock){
	mGUIM->updateElements(clock.restart());
	mEM->updateEntities(clock.getElapsedTime());
}

void GameStateLevel::render(sf::RenderWindow& window){

}