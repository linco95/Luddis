#include "GameStateLevel.h"
#include "ViewUtility.h"

GameStateLevel::GameStateLevel(sf::RenderWindow* window) :
mEM(&EntityManager::getInstance()),
mGUIM(&GUIManager::getInstance()),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window){

}

GameStateLevel::~GameStateLevel(){

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