#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "PowerupDisplay.h"
#include "Dialogue.h"
#include "Level.h"
#include "Dust.h"
#include "Chips.h"
#include "SpiderEgg.h"
#include "Silverfish.h"
#include "Spider.h"
#include "EventZone.h"
#include "Inventory.h"
#include "Luddis.h"
#include "Debug.h"

static const std::string LUDDIS_TEXTURE = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";

static const std::string POWER_DISPLAY = "Resources/Images/GUI/PowerButton.png";
static const std::string BUTTON_TEXTURE = "Resources/Images/GUI/Button.png";

GameStateLevel::GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager) :
mEntityM(entityManager),
mEventM(),
mGUIM(guiManager),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mWindow(window),
mInDialogue(false),
mResetView(false){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel::~GameStateLevel(){
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
}

void GameStateLevel::initialize(GameStatePaused* gameStateLevel){
	mGameStatePaused = gameStateLevel;
	mPowerupDisplays[0] = new PowerupDisplay(POWER_DISPLAY, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.8f, (float)ViewUtility::VIEW_HEIGHT - 60), 15.0f);
	mGUIM->addInterfaceElement(mPowerupDisplays[0]);
}

void GameStateLevel::update(sf::Clock& clock){
	//Do game logic
	if (!mInDialogue){
		mEntityM->updateEntities(clock.getElapsedTime());
		mCM->detectCollisions();
	}
	//Debug::log(std::to_string( clock.getElapsedTime().asSeconds())+ "\n" );
	if (mResetView) {
		mMapView = mWindow->getView();
		mWindow->setView(mGUIView);
		mResetView = false;
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

void GameStateLevel::createDialogue(std::string dialogueFilename){
	sf::Vector2f pos((float)ViewUtility::VIEW_WIDTH*0.3f, (float)ViewUtility::VIEW_HEIGHT - 100);
	Dialogue* dialogue = new Dialogue(dialogueFilename, mWindow, mGUIM, &mEventM, pos, this);
	mGUIM->addInterfaceElement(dialogue);
	if (dialogueFilename.substr() == "SpiderDialogue"){
		mSpider = new Spider(mWindow, sf::Vector2f(400, 5));
		mGUIM->addInterfaceElement(mSpider);
	}
	mInDialogue = true;
}

//Retarded name, but it returns if there is currently a dialogue
//playing or not.
bool GameStateLevel::getInDialogue() const{
	return mInDialogue;
}

void GameStateLevel::setInDialogue(bool inDialogue){
	mInDialogue = inDialogue;
	if (mSpider != nullptr){
		mSpider->turn();
		mSpider = nullptr;
	}
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
	mLevel = new Level(mEntityM, this);
	mEntityM->addEntity(mLevel);
	mLevel->initializeLevel(*mWindow, mPlayer, levelFile);
}

void GameStateLevel::resetLevel(){
	if (!mCurrentLevelFile.empty()){
		mResetView = true;
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

void GameStateLevel::setupMission(const std::string& mapFilename, const std::string& jsonFilename) {
	std::string configText = ResourceManager::getInstance().loadJsonFile(jsonFilename);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	mLevel->initializeEntities(mWindow, configDoc);
	mLevel->readInitMap(mapFilename);
}