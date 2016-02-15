#include "GameStatePaused.h"
#include "GameStateLevel.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "GameManager.h"

GameStatePaused::GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType, EntityManager* entityManager) :
mEntityM(entityManager),
mEventM(&EventManager::getInstance()),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window),
mMenu(window, Menu::PAUSEMENU, entityManager){
	mEventM->attatch(this, sf::Event::EventType::KeyPressed);
}

GameStatePaused::~GameStatePaused(){
	mEventM->detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStatePaused::initialize(GameStateLevel* gameStateLevel){
	mMenu.initialize(gameStateLevel);
	mMenu.initializeButtons(Menu::PAUSEMENU);
	mGameStateLevel = gameStateLevel;
}

void GameStatePaused::update(sf::Clock& clock){
	//Do game logic
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mMenu.tick(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStatePaused::render(){
	//Draw objects
	mEntityM->renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->renderElements(*mWindow);
	mWindow->draw(mMenu);
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStatePaused::onEvent(const sf::Event &aEvent){
	if (true){
		switch (aEvent.type){
		case (sf::Event::EventType::KeyPressed) :
			if (aEvent.key.code == sf::Keyboard::Escape){
			GameManager::getInstance().setGameState(mGameStateLevel);
			}
			break;
		}
	}
}