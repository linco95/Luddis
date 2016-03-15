#include "GameStateStart.h"
#include "Menu.h"
#include "ViewUtility.h"
#include "SoundEngine.h"
#include "GameStateMap.h"
#include "GameManager.h"
#include "Mannequin.h"
#include "ScoreCounter.h"
#include "ResourceManager.h"
#include "rapidjson/document.h"
#include <SFML/Window/Event.hpp>

static const char* TEXTURE_SCORE_DUST = "Resources/Images/GUI/HUD_Ludd_Icon.png";
static const char* TEXTURE_SCORE_CHIPS = "Resources/Images/GUI/HUD_Chips_Icon.png";

static const char* ARIAL_FONT = "Resources/Fonts/arial.ttf";

GameStateStart::GameStateStart() :
	mGUIM(),
	mEventM(),
	mSelectedSave(0),
	mMannequin(new Mannequin) {

	mMannequin->setActive(false);
	mMannequin->setScale(2.0f, 2.0f);
	mMannequin->setPosition(ViewUtility::getViewSize().getCenter());
	mGUIM.addInterfaceElement(mMannequin);
}

GameStateStart::~GameStateStart() {
	mGUIM.clearInterfaceElements();
}

GameStateStart & GameStateStart::getInstance() {
	static GameStateStart gss;
	return gss;
}

void GameStateStart::initialize(sf::RenderWindow* window) {
	mWindow = window;
	SoundEngine::getInstance().playEvent("event:/Music/Meny");
	mMenu = new Menu(mWindow, &mEventM, &mGUIM, Menu::MenuType::MAINMENU);
	mGUIM.addInterfaceElement(mMenu);
	mMenu->setActive(true);
	mMenu->initialize(this);
}

void GameStateStart::update(sf::Clock& clock) {
	//mWindow->setView(ViewUtility::getViewSize());
	//Do game logic
	mGUIM.updateElements(clock.restart());

	//Garbage collection
	mGUIM.removeObsoleteElements();
}

void GameStateStart::render() {
	//Draw objects
	//mWindow->draw(mBackground);
	Renderer::getInstance().render(*mWindow);
	mGUIM.renderElements(*mWindow);
	for (int i = 0; i < 4; i++)
		mWindow->draw(mFiles[i].text);
}

void GameStateStart::onEvent(const sf::Event& aEvent) {

}

void GameStateStart::handleEvents() {
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)) {
		mEventM.notify(currEvent);
	}
}

void GameStateStart::handleClicks(std::string command) {
	std::string fileSubstr = command.substr(0, 4);


	if (command == "NewGame") {
		//Just a copy of ExitLevel for now
		GameManager::getInstance().setGameState(&GameStateMap::getInstance());
		SoundEngine* se = &SoundEngine::getInstance();

		se->stopEvent("event:/Music/Meny", FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		se->playEvent("event:/Music/Sockshop");
	}
	else if (command == "Play") {
		for (auto m : mFiles) {
			m.mannequin->setActive(true);
			m.text.setString(m.name);
		}
		if (mMannequin != nullptr)
			mMannequin->setActive(true);
		if (mFiles[mSelectedSave].scoreCounters[0] != nullptr)
			mFiles[mSelectedSave].scoreCounters[0]->setActive(true);
		if (mFiles[mSelectedSave].scoreCounters[1] != nullptr)
			mFiles[mSelectedSave].scoreCounters[1]->setActive(true);
	}
	else if (command == "Previous") {
		for (auto m : mFiles) {
			m.mannequin->setActive(false);
			m.text.setString("");
		}
		if (mMannequin != nullptr)
			mMannequin->setActive(false);
		if (mFiles[mSelectedSave].scoreCounters[0] != nullptr)
			mFiles[mSelectedSave].scoreCounters[0]->setActive(false);
		if (mFiles[mSelectedSave].scoreCounters[1] != nullptr)
			mFiles[mSelectedSave].scoreCounters[1]->setActive(false);
	}
	else if (fileSubstr == "File") {
		std::string subString = command.substr(4, command.size());
		int index = std::stoi(subString);

		mFiles[mSelectedSave].scoreCounters[0]->setActive(false);
		mFiles[mSelectedSave].scoreCounters[1]->setActive(false);
		mSelectedSave = index;
		mFiles[mSelectedSave].scoreCounters[0]->setActive(true);
		mFiles[mSelectedSave].scoreCounters[1]->setActive(true);

		//Set mannequin accessories
		if (mMannequin != nullptr) {
			//mMannequin->setHeadAccessory("");
			//mMannequin->setTailAccessory("");
		}
	}
	else if (command == "ConfirmYes") {

	}
	else if (command == "ConfirmNo") {

	}
}

void GameStateStart::setupFiles(std::string filename) {
	std::string configText = ResourceManager::getInstance().getJsonFile(filename);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsArray());

	for (rapidjson::SizeType itr = 0; itr < configDoc.Size(); itr++) {
		assert(configDoc[itr].HasMember("NEW") && configDoc[itr]["NEW"].IsBool());
		assert(configDoc[itr].HasMember("Playername") && configDoc[itr]["Playername"].IsString());
		assert(configDoc[itr].HasMember("Inventory") && configDoc[itr]["Inventory"].IsObject());

		mFiles[itr].NEW = configDoc[itr]["NEW"].GetBool();
		if (mFiles[itr].NEW)
			mFiles[itr].name = "Nytt Spel";
		else
			mFiles[itr].name = configDoc[itr]["Playername"].GetString();

		mFiles[itr].text.setString(mFiles[itr].name);
		mFiles[itr].text.setCharacterSize(36);
		mFiles[itr].text.setFont(ResourceManager::getInstance().getFont(ARIAL_FONT));
		mFiles[itr].text.setOrigin(0, mFiles[itr].text.getGlobalBounds().height / 2.0f);
		mFiles[itr].text.setPosition(300.0f, 200.0f + (float)itr*225.0f);
		mFiles[itr].text.setString("");

		const rapidjson::Value& inventory = configDoc[itr]["Inventory"];
		assert(inventory.HasMember("CurrentDust") && inventory["CurrentDust"].IsInt());
		assert(inventory.HasMember("MaxDust") && inventory["MaxDust"].IsInt());
		assert(inventory.HasMember("Chips") && inventory["Chips"].IsInt());
		assert(inventory.HasMember("Accessories") && inventory["Accessories"].IsObject());

		if (inventory["CurrentDust"].GetInt() != 0 || inventory["Chips"].GetInt() != 0) {
			sf::Vector2f position(ViewUtility::getViewSize().getSize() / 2.0f);
			position.y += 200;
			mFiles[itr].scoreCounters[0] = new ScoreCounter(mWindow, TEXTURE_SCORE_DUST, position, ScoreCounter::ScoreType::DUST);
			mFiles[itr].scoreCounters[0]->setScore(inventory["CurrentDust"].GetInt());
			mFiles[itr].scoreCounters[0]->setActive(false);
			mGUIM.addInterfaceElement(mFiles[itr].scoreCounters[0]);
			position.y += 90;
			mFiles[itr].scoreCounters[1] = new ScoreCounter(mWindow, TEXTURE_SCORE_CHIPS, position, ScoreCounter::ScoreType::CHIPS);
			mFiles[itr].scoreCounters[1]->setScore(inventory["Chips"].GetInt());
			mFiles[itr].scoreCounters[1]->setActive(false);
			mGUIM.addInterfaceElement(mFiles[itr].scoreCounters[1]);
		}
		else {
			mFiles[itr].scoreCounters[0] = nullptr;
			mFiles[itr].scoreCounters[1] = nullptr;
		}

		const rapidjson::Value& accessories = inventory["Accessories"];
		assert(accessories.HasMember("Head") && accessories["Head"].IsInt());
		assert(accessories.HasMember("Tail") && accessories["Tail"].IsInt());
		mFiles[itr].mannequin = new Mannequin();
		mFiles[itr].mannequin->setAnimate(false);
		mFiles[itr].mannequin->setActive(false);
		mFiles[itr].mannequin->setScale(1.0f, 1.0f);
		mFiles[itr].mannequin->setPosition(125.0f, 200.0f + (float)itr*225.0f);
		mGUIM.addInterfaceElement(mFiles[itr].mannequin);
	}
}
