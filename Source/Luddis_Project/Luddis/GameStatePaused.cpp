#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "EventManager.h"

GameStatePaused::GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType, EntityManager* entityManager, GameState* currentGameState) :
mEM(entityManager),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window),
mMenu(window, menuType, this, mEM),
mCurrentGameState(currentGameState){
	EventManager::getInstance().attatch(this, sf::Event::EventType::KeyPressed);
}

GameStatePaused::~GameStatePaused(){
	EventManager::getInstance().detatch(this, sf::Event::EventType::KeyPressed);
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
	mEM->renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->renderElements(*mWindow);
	mWindow->draw(mMenu);
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStatePaused::onEvent(const sf::Event &aEvent){

}