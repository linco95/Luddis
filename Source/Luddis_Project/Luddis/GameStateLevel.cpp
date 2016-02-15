#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "PowerupDisplay.h"
#include "Dialogue.h"

static const std::string POWER_DISPLAY = "Resources/Images/PowerButton.png";
static const std::string BUTTON_TEXTURE = "Resources/Images/Button.png";
static const std::string TEST_DIALOGUE = "Resources/Configs/Dialogue/Example_Dialogue.json";

GameStateLevel::GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager) :
mEntityM(entityManager),
mEventM(),
mGUIM(guiManager),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel::~GameStateLevel(){
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStateLevel::initialize(GameStatePaused* gameStateLevel){
	mGameStatePaused = gameStateLevel;
	mPowerupDisplays[0] = new PowerupDisplay(POWER_DISPLAY, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.8f, (float)ViewUtility::VIEW_HEIGHT - 60), 15.0f);
	mGUIM->addInterfaceElement(mPowerupDisplays[0]);
	Dialogue* dialogue = new Dialogue(TEST_DIALOGUE, mWindow, mGUIM, &mEventM, sf::Vector2f(150, 150));
	mGUIM->addInterfaceElement(dialogue);
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

void GameStateLevel::handleEvents(){
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)){
		mEventM.notify(currEvent);
	}
}