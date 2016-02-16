#include "GameStatePaused.h"
#include "GameStateLevel.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GameManager.h"

GameStatePaused::GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType, EntityManager* entityManager, GUIManager* guiManager) :
mEntityM(entityManager),
mEventM(),
mLevelGUIM(guiManager),
mGUIView(ViewUtility::getViewSize()),
mWindow(window){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
	mMenu = new Menu(window, &mEventM, &mMenuGUIM, Menu::PAUSEMENU, entityManager);
	mMenu->setActive(true);
	mMenuGUIM.addInterfaceElement(mMenu);
}

GameStatePaused::~GameStatePaused(){
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
	delete mMenu;
}

void GameStatePaused::initialize(GameStateLevel* gameStateLevel){
	mMenu->initialize(gameStateLevel);
	mMenu->initializeButtons(Menu::PAUSEMENU);
	mGameStateLevel = gameStateLevel;
}

void GameStatePaused::update(sf::Clock& clock){
	//Do game logic

	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mMenuGUIM.updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStatePaused::render(){
	//Draw objects
	mEntityM->renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mLevelGUIM->renderElements(*mWindow);
	mMenuGUIM.renderElements(*mWindow);
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

void GameStatePaused::handleEvents(){
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)){
		mEventM.notify(currEvent);
	}
}