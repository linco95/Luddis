#include "GameStateMap.h"
#include <SFML/Window/Event.hpp>
#include "EventManager.h"
#include "EntityManager.h"
#include "GameManager.h"

GameStateMap::GameStateMap(sf::RenderWindow* window):
mWindow(window),
mEntityM(),
mGUIM(),
mEventM(){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateMap::~GameStateMap(){

}

void GameStateMap::update(sf::Clock& clock){
	//Do game logic
	mEntityM.updateEntities(clock.getElapsedTime());
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM.updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);


	//Garbage collection
	mEntityM.removeDeadEntities();
	mGUIM.removeObsoleteElements();
}

void GameStateMap::render(){
	//Draw objects
	mEntityM.renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM.renderElements(*mWindow);
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStateMap::onEvent(const sf::Event &aEvent){
	if (true){
		switch (aEvent.type){
		case (sf::Event::EventType::KeyPressed) :
			if (aEvent.key.code == sf::Keyboard::Escape){
			//GameManager::getInstance().setGameState(mGameStatePaused);
			}
												break;
		}
	}
}

void GameStateMap::handleEvents(){
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)){
		mEventM.notify(currEvent);
	}
}