#include "Shop.h"
#include "GameState.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "Inventory.h"
#include "rapidjson/document.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Debug.h"

const char* BACKGROUND_TEXTURE = "Resources/Images/GUI/ShopBackground.png";
const char* BUTTON_DIRECTION = "Resources/Images/GUI/DirectionButton.png";
const char* BUTTON_EXIT = "Resources/Images/Rooms/ButtonSockShopDone.png";

const char* FONT_NAME = "Resources/Fonts/trebuc.ttf";

const char* CONFIG_FILE = "Resources/Configs/Inventory/Accessories.json";

Shop::Shop(sf::RenderWindow* window, GameState* gameState, EventManager* eventManager, GUIManager* guiManager) :
	mWindow(window),
	mEventManager(eventManager),
	mGUIManager(guiManager),
	mGameState(gameState),
	mInventory(&Inventory::getInstance()),
	mButtons(),
	mChips("X " + std::to_string(mInventory->getChips()),
		ResourceManager::getInstance().getFont(FONT_NAME)),
	mActivePage(0),
	mMaxPage(0),
	mBackground(ResourceManager::getInstance().getTexture(BACKGROUND_TEXTURE)),
	mIsAlive(true),
	mIsActive(true) {

	sf::Vector2f origin((float)mBackground.getTextureRect().width / 2, (float)mBackground.getTextureRect().height / 2);
	mBackground.setOrigin(origin);
	mBackground.setPosition(ViewUtility::getViewSize().getCenter());
	mChips.setPosition(ViewUtility::getViewSize().getCenter() - sf::Vector2f(-100.0f, (float)mBackground.getTextureRect().height / 3.1f));
	mChips.setColor(sf::Color::Black);
	mChips.setCharacterSize(60);
	initialize();
}

Shop::~Shop() {
	while (!mButtons.empty()) {
		mButtons.back()->kill();
		mButtons.pop_back();
	}
	for (int i = 0; i < mMaxPage; i++) {
		while (!mItems[i].empty()) {
			mItems[i].back()->kill();
			mItems[i].pop_back();
		}
		while (!mDescriptions[i].empty()) {
			delete mDescriptions[i].back();
			mDescriptions[i].pop_back();
		}
	}
	mGameState->handleClicks("ShopDelete");
}

void Shop::initialize() {
	sf::Vector2f position = mBackground.getPosition();
	float offset = (float)mBackground.getTextureRect().width;
	position.x -= offset / 2;
	Button* button = new Button(BUTTON_DIRECTION, "", "Page-1", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	button->setScale(-1, 1);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);
	position.x += offset;
	button = new Button(BUTTON_DIRECTION, "", "Page1", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);
	position.x -= offset / 3.0f;
	position.y -= offset / 2.0f;
	button = new Button(BUTTON_EXIT, "", "Close", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);

	initializeContents();
}

void Shop::initializeContents() {
	std::string configString = ResourceManager::getInstance().getJsonFile(CONFIG_FILE);
	rapidjson::Document configDoc;
	configDoc.Parse(configString.c_str());

	assert(configDoc.IsArray());
	int counter = 0;
	for (rapidjson::SizeType itr = 0; itr < configDoc.Size(); itr++) {
		assert(configDoc[itr].HasMember("image"));
		assert(configDoc[itr].HasMember("description"));
		assert(configDoc[itr].HasMember("ID"));

		std::string image = configDoc[itr]["image"].GetString();
		std::string description = configDoc[itr]["description"].GetString();
		int ID = configDoc[itr]["ID"].GetInt();
		sf::Vector2f position = sf::Vector2f(176, (float)(486 + counter * 210)) + mBackground.getPosition() - mBackground.getOrigin();
		addItemButton(image, "Accessory" + std::to_string(ID), position, mMaxPage);
		sf::IntRect box(330, 418 + counter * 208, 382, 142);
		mDescriptions[mMaxPage].push_back(new TextBoxDecorator(box, description, 32));
		counter++;

		if (counter % 3 == 0) {
			counter = 0;
			mMaxPage++;
		}
	}

	for (auto d : mDescriptions[0]) {
		d->setActive(true);
	}
	for (auto i : mItems[0]) {
		i->setActive(true);
	}
}

void Shop::addItemButton(std::string buttonFile, std::string buttonFunc, sf::Vector2f pos, int index) {
	Button* button = new Button(buttonFile, "", buttonFunc, mWindow, mEventManager, pos, this, Button::RECTANGLE, Button::FIRST);
	mItems[index].push_back(button);
	mGUIManager->addInterfaceElement(button);
	button->setActive(false);
}

void Shop::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mChips, states);
}

void Shop::tick(const sf::Time & deltaTime) {
	mChips.setString("X " + std::to_string(mInventory->getChips()));
}

Shop::Strata Shop::getRenderLayer() const {
	return SECOND;
}

bool Shop::isAlive() const {
	return mIsAlive;
}

bool Shop::isActive() const {
	return mIsActive;
}

void Shop::setActive(const bool & active) {
	mIsActive = active;
}

void Shop::onClick(std::string command) {
	std::string pageSubstr = command.substr(0, 4);
	std::string accessorySubstr = command.substr(0, 9);

	if (pageSubstr == "Page") {
		int page = std::stoi(command.substr(pageSubstr.length(), command.length()));
		buttonFuncSwitchPage(page);
	}
	else if (command == "Close") {
		mIsAlive = false;
	}
	else if (accessorySubstr == "Accessory") {
		//Do things to luddis mannequin...horrible things
		Debug::log(command, Debug::INFO);
	}
}

void Shop::buttonFuncSwitchPage(int page) {
	//Outside
	if ((mActivePage + page) >= mMaxPage || (mActivePage + page) < 0)
		return;

	for (auto i : mItems[mActivePage])
		i->setActive(false);
	for (auto d : mDescriptions[mActivePage])
		d->setActive(false);
	mActivePage += page;
	for (auto i : mItems[mActivePage])
		i->setActive(true);
	for (auto d : mDescriptions[mActivePage])
		d->setActive(true);
}
