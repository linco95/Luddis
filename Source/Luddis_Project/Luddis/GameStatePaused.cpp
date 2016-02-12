#include "GameStatePaused.h"

GameStatePaused::GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType) :
mEM(&EntityManager::getInstance()),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(sf::FloatRect(0, 0, (float)WIDTH, (float)HEIGHT)),
mWindow(window),
mMenu(window, menuType, this){

}

GameStatePaused::~GameStatePaused(){

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