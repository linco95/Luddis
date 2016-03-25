#include "Shop.h"
#include "GameState.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "Inventory.h"
#include "Mannequin.h"
#include "rapidjson/document.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Debug.h"

const char* BACKGROUND_TEXTURE = "Resources/Images/GUI/ShopBackground.png";
const char* BUTTON_DIRECTION = "Resources/Images/GUI/DirectionButton.png";
const char* BUTTON_EXIT = "Resources/Images/Rooms/ButtonSockShopDone.png";

const char* FONT_NAME = "Resources/Fonts/trebuc.ttf";

const char* CONFIG_FILE = "Resources/Configs/Inventory/Accessories.json";

Shop::Shop(sf::RenderWindow* window, GameState* gameState, EventManager* eventManager, GUIManager* guiManager, Mannequin* mannequin) :
	mWindow(window),
	mEventManager(eventManager),
	mGUIManager(guiManager),
	mGameState(gameState),
	mMannequin(mannequin),
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
	mBackground.setPosition(ViewUtility::getViewSize().getSize().x / 3, ViewUtility::getViewSize().getCenter().y);
	mChips.setPosition(getPosition() - sf::Vector2f(-100.0f, (float)mBackground.getTextureRect().height / 3.1f));
	mChips.setColor(sf::Color::Black);
	mChips.setCharacterSize(60);

	if (mMannequin != nullptr) {
		mMannequin->reset();
		mMannequin->setAnimate(true);
		mMannequin->setActive(true);
	}

	initialize();
}

Shop::~Shop() {
	while (!mButtons.empty()) {
		mButtons.back()->kill();
		mButtons.pop_back();
	}
	for (int i = 0; i <= mMaxPage; i++) {
		while (!mItems[i].empty()) {
			mItems[i].back()->kill();
			mItems[i].pop_back();
		}
		while (!mDescriptions[i].empty()) {
			delete mDescriptions[i].back();
			mDescriptions[i].pop_back();
		}
	}
	Inventory::getInstance().setAccessoryHead(mMannequin->getHeadAccessory());
	Inventory::getInstance().setAccessoryTail(mMannequin->getTailAccessory());
	Inventory::getInstance().setColorScheme(mMannequin->getColorScheme());
	if (mMannequin != nullptr)
		mMannequin->setActive(false);

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
	button = new Button(BUTTON_EXIT, "", "Close", mWindow, mEventManager, position, this, Button::CIRCLE);
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
		if (counter == 3) {
			counter = 0;
			mMaxPage++;
		}
		assert(configDoc[itr].HasMember("icon") && configDoc[itr]["icon"].IsString());
		assert(configDoc[itr].HasMember("image") && configDoc[itr]["image"].IsString());
		assert(configDoc[itr].HasMember("description") && configDoc[itr]["description"].IsString());
		assert(configDoc[itr].HasMember("type") && configDoc[itr]["type"].IsInt());

		std::string icon = configDoc[itr]["icon"].GetString();
		std::string description = configDoc[itr]["description"].GetString();
		std::string image = configDoc[itr]["image"].GetString();
		int type = configDoc[itr]["type"].GetInt();
		sf::Vector2f position = sf::Vector2f(200, (float)(486 + counter * 200)) + mBackground.getPosition() - mBackground.getOrigin();
		addItemButton(icon, "Accessory" + std::to_string(itr), position, mMaxPage);
		sf::IntRect box(130, 0, 382, 142);
		TextBoxDecorator* textBox = new TextBoxDecorator(box, description, 32);
		textBox->setOrigin(0, (float)box.height / 2.0f);
		textBox->setPosition(position);
		mDescriptions[mMaxPage].push_back(textBox);
		mAccessoryImages[itr] = image;
		counter++;

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
	for (auto d : mDescriptions[mActivePage])
		target.draw(*d, states);
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
		int ID = std::stoi(command.substr(9));
		//Do things to luddis mannequin...horrible things
		switch (ID) {
		case 0:
			mMannequin->setColorScheme(Mannequin::ColorScheme::PINK);
			break;

		case 1:
			mMannequin->setColorScheme(Mannequin::ColorScheme::BLUE);
			break;

		case 2:
			mMannequin->setColorScheme(Mannequin::ColorScheme::GREEN);
			break;

		default:
			mMannequin->setHeadAccessory(mAccessoryImages[ID].c_str());
			break;
		}

		Debug::log(command, Debug::INFO);
	}
}

void Shop::buttonFuncSwitchPage(int page) {
	//Outside
	if ((mActivePage + page) > mMaxPage || (mActivePage + page) < 0)
		return;

	for (auto i : mItems[mActivePage])
		i->setActive(false);
	mActivePage += page;
	for (auto i : mItems[mActivePage])
		i->setActive(true);
}
