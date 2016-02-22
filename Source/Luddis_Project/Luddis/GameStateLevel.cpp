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
mSetupLevel(false),
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

//TODO: Dialogue will not be reset as it is in the GUI vector. Fix.
void GameStateLevel::createDialogue(std::string dialogueFilename){
	sf::Vector2f pos(0.0f, (float)ViewUtility::VIEW_HEIGHT);
	Dialogue* dialogue = new Dialogue(dialogueFilename, mWindow, mGUIM, &mEventM, pos, this);
	mGUIM->addInterfaceElement(dialogue);
	if (dialogueFilename.substr() == "SpiderDialogue"){
		mSpider = new Spider(mWindow, sf::Vector2f(400, 0));
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

	if (!mSetupLevel) {
		mSetupLevel = false;
		std::string setupFile = levelFile.substr(0, 32);
		setupFile += "Setup.json";
		readSetupFiles(setupFile);
	}
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
	if (mSpider != nullptr) {
		mSpider->turn();
		mSpider = nullptr;
	}
}

void GameStateLevel::readSetupFiles(const std::string& filename) {
	ResourceManager* rm = &ResourceManager::getInstance();
	rapidjson::Document configDoc;
	std::string configText = rm->loadJsonFile(filename);
	configDoc.Parse(configText.c_str());

	assert(configDoc.IsObject());
	assert(configDoc.HasMember("Audio") && configDoc["Audio"].IsArray());
	const rapidjson::Value& audio = configDoc["Audio"];
	for (rapidjson::Value::ConstValueIterator itr = audio.Begin(); itr != audio.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("filename"));
		std::string file = (*itr)["filename"].GetString();
		rm->loadSoundBuffer(file);
	}

	assert(configDoc.HasMember("Graphics"));
	const rapidjson::Value& graphics = configDoc["Graphics"];
	assert(graphics.HasMember("Background") && graphics["Background"].IsString());
	assert(graphics.HasMember("Segments")&&graphics["Segments"].IsInt());
	int seg = graphics["Segments"].GetInt();
	for (int i = 0; i < seg; i++) {
		std::string file = graphics["Background"].GetString() + std::to_string(i+1) + ".png";
		rm->loadTexture(file);
	}

	assert(graphics.HasMember("Enemies")&&graphics["Enemies"].IsArray());
	const rapidjson::Value& enemies = graphics["Enemies"];

	for (rapidjson::Value::ConstValueIterator itr = enemies.Begin(); itr != enemies.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("filename"));
		std::string file = (*itr)["filename"].GetString();
		rm->loadTexture(file);
	}

	assert(configDoc.HasMember("Setup_files"));
	const rapidjson::Value& setupFiles = configDoc["Setup_files"];
	assert(setupFiles.HasMember("JSON_files")&& setupFiles["JSON_files"].IsArray());
	const rapidjson::Value& jsonFiles = setupFiles["JSON_files"];

	for (rapidjson::Value::ConstValueIterator itr = jsonFiles.Begin(); itr != jsonFiles.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("filename")&&(*itr)["filename"].IsString());
		std::string file = (*itr)["filename"].GetString();
		rm->loadJsonFile(file);
	}

	assert(setupFiles.HasMember("PNG_files") && setupFiles["PNG_files"].IsArray());
	const rapidjson::Value& pngFiles = setupFiles["PNG_files"];

	for (rapidjson::Value::ConstValueIterator itr = pngFiles.Begin(); itr != pngFiles.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
		std::string file = (*itr)["filename"].GetString();
		rm->readMap(file);
	}

}