#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "PowerupDisplay.h"
#include "Dialogue.h"
#include "Level.h"
#include "Inventory.h"
#include "Luddis.h"

static const std::string LUDDIS_TEXTURE = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";

static const std::string POWER_DISPLAY = "Resources/Images/GUI/PowerButton.png";
static const std::string BUTTON_TEXTURE = "Resources/Images/GUI/Button.png";
static const std::string TEST_DIALOGUE = "Resources/Configs/Dialogue/Example_Dialogue.json";

GameStateLevel::GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager) :
mEntityM(entityManager),
mEventM(),
mGUIM(guiManager),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window),
mInDialogue(false){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel::~GameStateLevel(){
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStateLevel::initialize(GameStatePaused* gameStateLevel){
	mGameStatePaused = gameStateLevel;
	mPowerupDisplays[0] = new PowerupDisplay(POWER_DISPLAY, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.8f, (float)ViewUtility::VIEW_HEIGHT - 60), 15.0f);
	mGUIM->addInterfaceElement(mPowerupDisplays[0]);
	Dialogue* dialogue = new Dialogue(TEST_DIALOGUE, mWindow, mGUIM, &mEventM, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.3f, (float)ViewUtility::VIEW_HEIGHT-100));
	mGUIM->addInterfaceElement(dialogue);
}

void GameStateLevel::update(sf::Clock& clock){
	//Do game logic
	if (!mInDialogue){
		mEntityM->updateEntities(clock.getElapsedTime());
		mCM->detectCollisions();
	}
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mGUIM->updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);

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

bool GameStateLevel::getInDialogue() const{
	return mInDialogue;
}

void GameStateLevel::setInDialogue(bool inDialogue){
	mInDialogue = inDialogue;
}

void GameStateLevel::setupLevel(std::string levelFile){
	Inventory* inv = &Inventory::getInstance();
	mInv.chips = inv->getChips();
	mInv.dust = inv->getDust();
	mInv.eggs = inv->getEggs();
	mCurrentLevelFile = levelFile;
	Luddis *mPlayer = new Luddis(LUDDIS_TEXTURE, mWindow, mEntityM);
	mEntityM->addEntity(mPlayer);
	mCM->addCollidable(mPlayer);
	mLevel = new Level(mEntityM);
	mEntityM->addEntity(mLevel);
	mLevel->initializeLevel(*mWindow, mPlayer, levelFile);
}

void GameStateLevel::resetLevel(){
	if (!mCurrentLevelFile.empty()){
		resetInventory();
		mCM->emptyVector();
		mEntityM->emptyVector();
		setupLevel(mCurrentLevelFile);
	}
}

void GameStateLevel::resetInventory(){
	Inventory* inv = &Inventory::getInstance();
	inv->setChips(mInv.chips);
	inv->setDust(mInv.dust);
	inv->setEggs(mInv.eggs);
}