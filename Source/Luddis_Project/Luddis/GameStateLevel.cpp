#include "GameStateLevel.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GameStatePaused.h"
#include "EventManager.h"

GameStateLevel::GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GameState* currentGameState) :
mEM(entityManager),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window),
mCurrentGameState(currentGameState){
	EventManager::getInstance().attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel::~GameStateLevel(){
	EventManager::getInstance().detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStateLevel::update(sf::Clock& clock){
	//Do game logic
	mEM->updateEntities(clock.getElapsedTime());
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);
	mCM->detectCollisions();
	

	//Garbage collection
	mCM->removeDeadCollidables();
	mEM->removeDeadEntities();
	mGUIM->removeObsoleteElements();
}

void GameStateLevel::render(){
	//Draw objects
	mEM->renderEntities(*mWindow);
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
	switch (aEvent.type){
	case (sf::Event::EventType::Closed) :
		break;
	case (sf::Event::EventType::KeyPressed) :
		if (aEvent.key.code == sf::Keyboard::Escape){
		//delete mCurrentGameState;
		//mCurrentGameState = new GameStatePaused(mWindow, Menu::PAUSEMENU, mEM, mCurrentGameState);
		}
		break;
	default:
		// NO-OP
		break;
	}
}