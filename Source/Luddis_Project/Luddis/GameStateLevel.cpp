#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "GameManager.h"

GameStateLevel::GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager) :
mEntityM(entityManager),
mEventM(&EventManager::getInstance()),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window){
	mEventM->attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel::~GameStateLevel(){
	mEventM->detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStateLevel::initialize(GameStatePaused* gameStateLevel){
	mGameStatePaused = gameStateLevel;
}

void GameStateLevel::update(sf::Clock& clock){
	//Do game logic
	mEntityM->updateEntities(clock.getElapsedTime());
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);
	mCM->detectCollisions();
	

	//Garbage collection
	mCM->removeDeadCollidables();
	mEntityM->removeDeadEntities();
	mGUIM->removeObsoleteElements();
}

void GameStateLevel::render(){
	//Draw objects
	mEntityM->renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->renderElements(*mWindow);
	//Then change it back
	mWindow->setView(mMapView); 
#ifdef LUDDIS_DEBUG_DRAW_HITBOXES
	mCM->drawHitboxes(*mWindow);
#endif
}

void GameStateLevel::onEvent(const sf::Event &aEvent){
	if (true){
		switch (aEvent.type){
		case (sf::Event::EventType::KeyPressed) :
			if (aEvent.key.code == sf::Keyboard::Escape){
			GameManager::getInstance().setGameState(mGameStatePaused);
			}
			break;
		}
	}
}