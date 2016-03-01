#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"
#include "EntityManager.h"
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
#include "ScoreGauge.h"
#include "Inventory.h"
#include "Luddis.h"
#include "LuddisStateCinematic.h"
#include "CinematicPause.h"

static const std::string LUDDIS_TEXTURE = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";
static const std::string POWER_DISPLAY = "Resources/Images/GUI/PowerButton.png";
static const std::string BUTTON_TEXTURE = "Resources/Images/GUI/Button.png";
static const std::string COMMON_RESOURCES = "Resources/Configs/Levels/CommonResources.json";

static const char* TEXTURE_LUDDGAUGE_FRAME = "Resources/Images/GUI/LuddGaugeFrame.png";
static const char* TEXTURE_LUDDGAUGE_BAR = "Resources/Images/GUI/LuddGaugeBar.png";


static const char* LOADINGBAR_FRAME = "Resources/Images/GUI/Loadingbar_Frame.png";
static const char* LOADINGBAR_BAR = "Resources/Images/GUI/Loadingbar_Bar.png";

GameStateLevel::GameStateLevel() :
mEventM(),
mResettableGUI(),
mCM(&CollisionManager::getInstance()),
mGUIView(ViewUtility::getViewSize()),
mInDialogue(false),
mFirstTime(true),
mResetView(false){
	mEventM.attatch(this, sf::Event::EventType::KeyPressed);
	readSetupFiles(COMMON_RESOURCES);
}

GameStateLevel::~GameStateLevel(){

	mGUIM->clearInterfaceElements();
	mEntityM->emptyVector();
	mEventM.detatch(this, sf::Event::EventType::KeyPressed);
}

GameStateLevel& GameStateLevel::getInstance() {
	static GameStateLevel gs;
	return gs;
}

void GameStateLevel::updateLuddGauge() {
	int maxDust = Inventory::getInstance().getMaxDust();
	int currentDust = Inventory::getInstance().getDust();
	float fillPercent = (float)currentDust / (float)maxDust;

	mLuddGauge->updateGauge(fillPercent);
}

void GameStateLevel::initialize(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager){
	mWindow = window;
	mEntityM = entityManager;
	mGUIM = guiManager;
	//TODO: Move to levelSetup(), as theese can change when selecting level.
	mPowerupDisplays[0] = new PowerupDisplay(POWER_DISPLAY, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.8f, (float)ViewUtility::VIEW_HEIGHT - 60), 15.0f);

	mLuddGauge = new ScoreGauge(mWindow, TEXTURE_LUDDGAUGE_FRAME, TEXTURE_LUDDGAUGE_BAR, sf::Vector2f(ViewUtility::VIEW_WIDTH * 0.45f, ViewUtility::VIEW_HEIGHT - 60));
	mGUIM->addInterfaceElement(mLuddGauge);

	mGUIM->addInterfaceElement(mPowerupDisplays[0]);
	mGameStatePaused = &GameStatePaused::getInstance();
}

void GameStateLevel::update(sf::Clock& clock){
	//Do game logic
	updateLuddGauge();
	if (!mInDialogue){
		mEntityM->updateEntities(clock.getElapsedTime());
		mCM->detectCollisions();
	}
	if (mResetView) {
		mMapView = mWindow->getView();
		mWindow->setView(mGUIView);
		mResetView = false;
	}
	//Change the view when updating GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mResettableGUI.updateElements(clock.getElapsedTime());
	mGUIM->updateElements(clock.restart());
	//Then change it back
	mWindow->setView(mMapView);

	//Look to see if Luddis is dead, before garbage collection
	if (!mPlayer->isAlive()) {
		mPlayable = false;
		mGameStatePaused->createMenu(Menu::DEATHMENU);
		GameManager::getInstance().setGameState(mGameStatePaused);
	}

	//Garbage collection
	mCM->removeDeadCollidables();
	mEntityM->removeDeadEntities();
	mGUIM->removeObsoleteElements();
	mResettableGUI.removeObsoleteElements();
}

void GameStateLevel::render(){
	//Draw objects
	mEntityM->renderEntities(*mWindow);
	//Change the view when drawing GUI elements
	mMapView = mWindow->getView();
	mWindow->setView(mGUIView);
	mResettableGUI.renderElements(*mWindow);
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
				mGameStatePaused->createMenu(Menu::PAUSEMENU);
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
	sf::Vector2f pos(0.0f, (float)ViewUtility::VIEW_HEIGHT);
	Dialogue* dialogue = new Dialogue(dialogueFilename, mWindow, &mResettableGUI, &mEventM, pos);
	mResettableGUI.addInterfaceElement(dialogue);
	if (dialogueFilename.find("SpiderDialogue") != std::string::npos){
		mSpider = new Spider(mWindow, sf::Vector2f((float)ViewUtility::VIEW_WIDTH*0.6f, 0));
		mResettableGUI.addInterfaceElement(mSpider);
	}
	mInDialogue = true;
}

void GameStateLevel::fuckOffSpider() {
	if (mSpider != nullptr) {
		mSpider->turn();
		mSpider = nullptr;
	}
}

//Retarded name, but it returns if there is currently a dialogue
//playing or not.
bool GameStateLevel::getInDialogue() const{
	return mInDialogue;
}

void GameStateLevel::setInDialogue(bool inDialogue){
	mInDialogue = inDialogue;
	
}

void GameStateLevel::setupLevel(std::string levelFile) {
	if (mFirstTime) {
		mFirstTime = false;
		mCurrentLevelFile = levelFile;
	}
	

	mResetView = true;
	mInDialogue = false;

	Inventory* inv = &Inventory::getInstance();
	mCM->emptyVector();
	mEntityM->emptyVector();
	mResettableGUI.clearInterfaceElements();
	mInv.chips = inv->getChips();
	mInv.dust = inv->getDust();
	mInv.eggs = inv->getEggs();
	mPlayer = new Luddis(LUDDIS_TEXTURE, mWindow, mEntityM);
	//CINEMATIC TEST
	Polynomial poly;
	poly.addTerm(-1, 3);
	poly.addTerm(1, 2);
	poly.addTerm(1, 1);
	Tween tween(poly, 0, 2);
	Tween tween2(poly, 0, -2, false);
	CinematicPause pauseCin(1);
	LuddisStateCinematic* cinState = new LuddisStateCinematic(100, mPlayer, mWindow, mEntityM);
	/*cinState->addCinematicSequence(&tween);
	cinState->addCinematicSequence(&pauseCin);
	cinState->addCinematicSequence(&tween);*/
	cinState->addCinematicSequence(&tween2);
	cinState->addSpeedShift(50, 1);
	cinState->addSpeedShift(100, 1);
	cinState->addSpeedShift(50, 1);
	cinState->addSpeedShift(100, 1);
	cinState->addSpeedShift(50, 1);
	cinState->addSpeedShift(100, 1);
	mPlayer->setPlayerState(cinState);
	mPlayer->setPosition(-50.0f, (float)ViewUtility::VIEW_HEIGHT/2.0f);
	//END CINEMATIC TEST
	mEntityM->addEntity(mPlayer);
	mCM->addCollidable(mPlayer);
	mLevel = new Level(mEntityM);
	mEntityM->addEntity(mLevel);
	mLevel->initializeLevel(*mWindow, mPlayer, levelFile);

	//Pre-allocate files into memory if needed
	std::string setupFile = levelFile.substr(0, 32) + "Setup.json";
	std::string setupFileOld = mCurrentLevelFile.substr(0, 32) + "Setup.json";
	if (levelFile == mCurrentLevelFile)
		readSetupFiles(setupFile);
	else {
		readSetupFiles(setupFileOld, false);
		readSetupFiles(setupFile);
	}

	mCurrentLevelFile = levelFile;
	mPlayable = true;
}

void GameStateLevel::resetLevel(){
	if (!mCurrentLevelFile.empty()){
		resetInventory();
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
	//mLevel->readInitMap(mapFilename);
}

void GameStateLevel::readSetupFiles(const std::string& filename, bool allocate) {
	ResourceManager* rm = &ResourceManager::getInstance();
	rapidjson::Document configDoc;
	std::string configText = rm->loadJsonFile(filename);
	configDoc.Parse(configText.c_str());

	//TODO: Create a pretty loading bar
	
	//ScoreGauge* loadingbar = new ScoreGauge(mWindow, LOADINGBAR_FRAME, LOADINGBAR_BAR, ViewUtility::getViewSize().getSize()/0.5f, false);
	//mGUIM->addInterfaceElement(loadingbar);

	assert(configDoc.IsObject());
	assert(configDoc.HasMember("Audio") && configDoc["Audio"].IsArray());
	const rapidjson::Value& audio = configDoc["Audio"];
	for (rapidjson::Value::ConstValueIterator itr = audio.Begin(); itr != audio.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("filename"));
		std::string file = (*itr)["filename"].GetString();
		/*if (allocate) TODO : RE-implement.
			rm->loadSoundBuffer(file);
		else
			rm->clearSoundBuffer(file);*/
	}

	assert(configDoc.HasMember("Graphics"));
	const rapidjson::Value& graphics = configDoc["Graphics"];
	if (graphics.HasMember("Background") && graphics["Background"].IsString()) {
		assert(graphics.HasMember("Segments") && graphics["Segments"].IsInt());
		int seg = graphics["Segments"].GetInt();
		for (int i = 0; i < seg; i++) {
			std::string file = graphics["Background"].GetString() + std::to_string(i + 1) + ".png";
			if (allocate)
				rm->loadTexture(file);
			else
				rm->clearTexture(file);
		}
	}

	if (graphics.HasMember("Sprites") && graphics["Sprites"].IsArray()) {
		const rapidjson::Value& sprites = graphics["Sprites"];

		for (rapidjson::Value::ConstValueIterator itr = sprites.Begin(); itr != sprites.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("filename"));
			std::string file = (*itr)["filename"].GetString();
			if (allocate)
				rm->loadTexture(file);
			else {
				rm->clearTexture(file);
			}
		}
	}

	if (graphics.HasMember("Spritesheets") && graphics["Spritesheets"].IsArray()) {
		const rapidjson::Value& spritesheets = graphics["Spritesheets"];

		for (rapidjson::Value::ConstValueIterator itr = spritesheets.Begin(); itr != spritesheets.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("filename"));
			std::string file((*itr)["filename"].GetString());
			if (allocate) {
				rm->loadJsonFile(file + ".json");
				rm->loadTexture(file + ".png");
			}
			else {
				rm->clearJsonFile(file + ".json");
				rm->clearTexture(file + ".png");
			}
		}
	}

	if (graphics.HasMember("Sprites") && graphics["Sprites"].IsArray()) {
		const rapidjson::Value& sprites = graphics["Sprites"];

		for (rapidjson::Value::ConstValueIterator itr = sprites.Begin(); itr != sprites.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("filename"));
			std::string file = (*itr)["filename"].GetString();
			if (allocate)
				rm->loadTexture(file);
			else {
				rm->clearTexture(file);
			}
		}
	}
	if (configDoc.HasMember("Setup_files")) {
		const rapidjson::Value& setupFiles = configDoc["Setup_files"];
		assert(setupFiles.HasMember("JSON_files") && setupFiles["JSON_files"].IsArray());
		const rapidjson::Value& jsonFiles = setupFiles["JSON_files"];

		for (rapidjson::Value::ConstValueIterator itr = jsonFiles.Begin(); itr != jsonFiles.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
			std::string file = (*itr)["filename"].GetString();
			if (allocate)
				rm->loadJsonFile(file);
			else
				rm->clearJsonFile(file);
		}

		assert(setupFiles.HasMember("PNG_files") && setupFiles["PNG_files"].IsArray());
		const rapidjson::Value& pngFiles = setupFiles["PNG_files"];

		for (rapidjson::Value::ConstValueIterator itr = pngFiles.Begin(); itr != pngFiles.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
			std::string file = (*itr)["filename"].GetString();

			if (allocate)
				rm->readMap(file);
			else
				rm->clearMap(file);
		}
	}
	//loadingbar->kill();
}

bool GameStateLevel::playable() const {
	return mPlayable;
}