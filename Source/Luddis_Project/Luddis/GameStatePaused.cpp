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
	mMenu->kill();
	mMenuGUIM.clearInterfaceElements();
	//mBackgroundGUIM->clearInterfaceElements();
}

GameStatePaused& GameStatePaused::getInstance() {
	static GameStatePaused gs;
	return gs;
}

void GameStatePaused::initialize(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager){
	mLastGameState = &GameStateLevel::getInstance();
	mWindow = window;
	mEntityM = entityManager;
	mBackgroundGUIM = guiManager;
}

void GameStatePaused::setBackgroundParameters(EntityManager * entityManager, GUIManager * guiManager, GameState* gameState){
	mEntityM = entityManager;
	mBackgroundGUIM = guiManager;
	mLastGameState = gameState;
}

void GameStatePaused::createMenu(Menu::MenuType menuType, GameState* lastGameState) {

	if (mMenu != nullptr)
		mMenu->kill();

	mMenu = new Menu(mWindow, &mEventM, &mMenuGUIM, menuType);
	mMenuGUIM.addInterfaceElement(mMenu);
	mMenu->setActive(true);
	mMenu->initialize(lastGameState);

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
	//Draw objects (unless null pointer)
	if (mEntityM != nullptr)
		mEntityM->renderEntities(*mWindow);
	Renderer::getInstance().render(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mBackgroundGUIM->renderElements(*mWindow);
	mMenuGUIM.renderElements(*mWindow);
	//Then change it back
	mWindow->setView(mMapView);
}

void GameStatePaused::onEvent(const sf::Event &aEvent) {
	if (true) {
		switch (aEvent.type) {
		case sf::Event::EventType::KeyPressed:
			if (aEvent.key.code == sf::Keyboard::Escape) {
				mMenu->kill();
				GameManager::getInstance().setGameState(mLastGameState);
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

void GameStatePaused::handleClicks(std::string command){

}
