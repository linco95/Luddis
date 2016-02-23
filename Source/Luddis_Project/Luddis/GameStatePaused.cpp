#include "GameStatePaused.h"
#include "GameStateLevel.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GameManager.h"

GameStatePaused::GameStatePaused() :
mEventM(),
mGUIView(ViewUtility::getViewSize()){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
}

GameStatePaused::~GameStatePaused(){
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
	delete mMenu;
}

GameStatePaused& GameStatePaused::getInstance() {
	static GameStatePaused gs;
	return gs;
}

void GameStatePaused::initialize(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager){
	mGameStateLevel = &GameStateLevel::getInstance();
	mWindow = window;
	mEntityM = entityManager;
	mLevelGUIM = guiManager;
}

void GameStatePaused::createMenu(Menu::MenuType menuType) {

	if (mMenu != nullptr)
		mMenu->kill();

	mMenu = new Menu(mWindow, &mEventM, &mMenuGUIM, menuType, mEntityM);
	mMenuGUIM.addInterfaceElement(mMenu);
	mMenu->setActive(true);
	mMenu->initialize();

}

void GameStatePaused::update(sf::Clock& clock){
	//Do game logic

	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mMenuGUIM.updateElements(clock.restart());

	//Then change it back
	mWindow->setView(mMapView);

	//Garbage collection
	mMenuGUIM.removeObsoleteElements();
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
			if (aEvent.key.code == sf::Keyboard::Escape && mGameStateLevel->playable()) {
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