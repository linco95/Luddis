#include "GameStateStart.h"
#include "Menu.h"
#include "ViewUtility.h"
#include "SoundEngine.h"
#include "GameStateMap.h"
#include "GameManager.h"
#include "Mannequin.h"
#include "ScoreCounter.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include "rapidjson/document.h"
#include <SFML/Window/Event.hpp>

static const char* TEXTURE_SCORE_DUST = "Resources/Images/GUI/HUD_Ludd_Icon.png";
static const char* TEXTURE_SCORE_CHIPS = "Resources/Images/GUI/HUD_Chips_Icon.png";
static const char* TEXTURE_LOGO = "Resources/Images/Luddis_logo.png";
static const char* TEXTURE_BACKROUND = "Resources/Images/Startmenu_background.jpg";
static const char* ACCESSORIES_LIST = "Resources/Configs/Inventory/Accessories.json";

static const char* DEFAULT_FONT = "Resources/Fonts/phitradesign Handwritten-Thin.ttf";

GameStateStart::GameStateStart() :
	mGUIM(),
	mEventM(),
	mSelectedSave(0),
	mLogo(ResourceManager::getInstance().getTexture(TEXTURE_LOGO)),
	mBackground(ResourceManager::getInstance().getTexture(TEXTURE_BACKROUND)),
	mMannequin(new Mannequin) {


	mLogo.setScale(1.0f, 1.0f);
	mLogo.setOrigin(mLogo.getLocalBounds().width / 2, mLogo.getLocalBounds().height / 2);
	mLogo.setPosition((float)ViewUtility::VIEW_WIDTH*0.5f, (float)ViewUtility::VIEW_HEIGHT*0.3f);
	mMannequin->setActive(false);
	mMannequin->setPosition(ViewUtility::getViewSize().getCenter());
	mMannequin->setScale(2.0f, 2.0f);
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
	Renderer::getInstance().render(*mWindow);
	mWindow->draw(mBackground);
	mWindow->draw(mLogo);
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

	//Very missleading, but this will use the currently selected savefile.
	if (command == "NewGame") {
		Inventory* inv = &Inventory::getInstance();
		inv->setChips(mFiles[mSelectedSave].chips);
		inv->setDust(mFiles[mSelectedSave].currentDust);
		inv->setMaxDust(mFiles[mSelectedSave].maxDust);
		inv->setAccessoryHead(mFiles[mSelectedSave].mannequin->getHeadAccessory());
		inv->setAccessoryTail(mFiles[mSelectedSave].mannequin->getTailAccessory());
		inv->setColorScheme(mFiles[mSelectedSave].mannequin->getColorScheme());
		GameManager::getInstance().setGameState(&GameStateMap::getInstance());
		SoundEngine* se = &SoundEngine::getInstance();

		se->stopEvent("event:/Music/Meny", FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		se->playEvent("event:/Music/Sockshop");
		se->playEvent("event:/Gameplay/Menu/Start_Game");

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

		if (mFiles[mSelectedSave].scoreCounters[0] != nullptr &&
			mFiles[mSelectedSave].scoreCounters[1] != nullptr) {
			mFiles[mSelectedSave].scoreCounters[0]->setActive(false);
			mFiles[mSelectedSave].scoreCounters[1]->setActive(false);
		}
		mSelectedSave = index;
		if (mFiles[mSelectedSave].scoreCounters[0] != nullptr &&
			mFiles[mSelectedSave].scoreCounters[1] != nullptr) {
			mFiles[mSelectedSave].scoreCounters[0]->setActive(true);
			mFiles[mSelectedSave].scoreCounters[1]->setActive(true);
		}

		//Set mannequin accessories
		if (mMannequin != nullptr) {
			std::string headAccessory, tailAccessory;
			Mannequin::ColorScheme color;

			headAccessory = mFiles[index].mannequin->getHeadAccessory();
			tailAccessory = mFiles[index].mannequin->getTailAccessory();
			color = mFiles[index].mannequin->getColorScheme();

			mMannequin->setHeadAccessory(headAccessory.c_str());
			mMannequin->setTailAccessory(tailAccessory.c_str());
			mMannequin->setColorScheme(color);
			mMannequin->setScale(2.0f, 2.0f);
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

	std::string configTextAccessories = ResourceManager::getInstance().getJsonFile(ACCESSORIES_LIST);
	rapidjson::Document configDocAccessories;
	configDocAccessories.Parse(configTextAccessories.c_str());
	assert(configDocAccessories.IsArray());

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
		mFiles[itr].text.setFont(ResourceManager::getInstance().getFont(DEFAULT_FONT));
		mFiles[itr].text.setOrigin(0, mFiles[itr].text.getGlobalBounds().height / 2.0f);
		mFiles[itr].text.setPosition(300.0f, 200.0f + (float)itr*225.0f);
		mFiles[itr].text.setString("");

		const rapidjson::Value& inventory = configDoc[itr]["Inventory"];
		assert(inventory.HasMember("CurrentDust") && inventory["CurrentDust"].IsInt());
		assert(inventory.HasMember("MaxDust") && inventory["MaxDust"].IsInt());
		assert(inventory.HasMember("Chips") && inventory["Chips"].IsInt());
		assert(inventory.HasMember("Accessories") && inventory["Accessories"].IsObject());

		if (inventory["CurrentDust"].GetInt() != 0 || inventory["Chips"].GetInt() != 0) {
			mFiles[itr].chips = inventory["Chips"].GetInt();
			mFiles[itr].maxDust = inventory["MaxDust"].GetInt();
			mFiles[itr].currentDust = inventory["CurrentDust"].GetInt();
			sf::Vector2f position(ViewUtility::getViewSize().getSize() / 2.0f);
			position.y += 200;
			mFiles[itr].scoreCounters[0] = new ScoreCounter(mWindow, TEXTURE_SCORE_DUST, position, ScoreCounter::ScoreType::DUST);
			mFiles[itr].scoreCounters[0]->setScore(mFiles[itr].currentDust);
			mFiles[itr].scoreCounters[0]->setActive(false);
			mGUIM.addInterfaceElement(mFiles[itr].scoreCounters[0]);
			position.y += 90;
			mFiles[itr].scoreCounters[1] = new ScoreCounter(mWindow, TEXTURE_SCORE_CHIPS, position, ScoreCounter::ScoreType::CHIPS);
			mFiles[itr].scoreCounters[1]->setScore(mFiles[itr].chips);
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
		assert(accessories.HasMember("Color") && accessories["Color"].IsInt());
		int headID = accessories["Head"].GetInt();
		int tailID = accessories["Tail"].GetInt();
		int colorID = accessories["Color"].GetInt();

		mFiles[itr].mannequin = new Mannequin();
		//mFiles[itr].mannequin->setAnimate(false);
		mFiles[itr].mannequin->setActive(false);
		mFiles[itr].mannequin->setScale(1.5f, 1.5f);
		mFiles[itr].mannequin->setPosition(125.0f, 200.0f + (float)itr*225.0f);

		for (rapidjson::SizeType itr2 = 0; itr2 < configDocAccessories.Size(); itr2++) {
			assert(configDocAccessories[itr2].HasMember("type") && configDocAccessories[itr2]["type"].IsInt());
			int accessoryType = configDocAccessories[itr2]["type"].GetInt();
			if (colorID != -1 && itr2 == colorID) {
				mFiles[itr].mannequin->setColorScheme(Mannequin::ColorScheme(itr2));
			}
			if (headID != -1 && itr2 == headID) {
				std::string image = configDocAccessories[itr2]["image"].GetString();
				mFiles[itr].mannequin->setHeadAccessory(image.c_str());
			}
			if (tailID != -1 && itr2 == tailID) {
				std::string image = configDocAccessories[itr2]["image"].GetString();
				mFiles[itr].mannequin->setHeadAccessory(image.c_str());
			}
		}

		mGUIM.addInterfaceElement(mFiles[itr].mannequin);
	}
}
