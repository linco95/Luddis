#include "Room.h"
#include "ViewUtility.h"
#include "GUIManager.h"
#include "ResourceManager.h"
#include "GameStateLevel.h"
#include "GameStateMap.h"
#include "GameManager.h"
#include "Dialogue.h"
#include "Filter.h"
#include "Debug.h"
#include <SFML/Graphics/Rect.hpp>

static const std::string LEVEL_CONFIG_PATH = "Resources/Configs/Levels/";
static const std::string DOOR_TEXTURE = "Resources/Images/Rooms/Doorknob.png";
static const std::string SHOP_TEXTURE = "Resources/Images/GUI/Shop.png";
static const std::string SOCKSHOP_TEXTURE = "Resources/Images/Rooms/Shop.png";
static const std::string DIALOGUE_TEXTURE = "Resources/Images/GUI/Dialogue.png";
static const std::string LEVEL1_TEXTURE = "Resources/Images/Rooms/Level1.png";
static const std::string LEVEL2_TEXTURE = "Resources/Images/Rooms/Level2.png";

static const std::string DIALOGUE_PATH = "Resources/Configs/Dialogue/";

Room::Room(GUIManager* guiManager, std::string textureFilename, EventManager* eventManager, sf::RenderWindow* window, GameState* gameState) :
	mIsActive(false),
	mIsAlive(true),
	mBackground(ResourceManager::getInstance().getTexture(textureFilename)),
	mGUIManager(guiManager),
	mEventManager(eventManager),
	mWindow(window),
	mGameState(gameState) {

}

Room::~Room() {
	for (auto b : mLevelButtons) {
		b->kill();
	}
}

void Room::tick(const sf::Time& deltaTime) {

}

void Room::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mIsActive) {
		states.transform *= getTransform();
		target.draw(mBackground, states);
	}
}

Room::Strata Room::getRenderLayer() const {
	return FIFTH;
}

bool Room::isAlive() const {
	return mIsAlive;
}

bool Room::isActive() const {
	return mIsActive;
}

void Room::setActive(const bool& active) {
	for (auto b : mLevelButtons)
		b->setActive(active);
	mIsActive = active;
}

void Room::kill() {
	mIsAlive = false;
}

void Room::onClick(std::string buttonFunc) {
	std::string levelSubstr = buttonFunc.substr(0, 5);
	std::string roomSubstr = buttonFunc.substr(0, 4);
	if (buttonFunc == "Shop") {
		buttonFuncShop();
	}
	else if (buttonFunc == "Dialogue") {
		buttonFuncDialogue();
	}
	else if (levelSubstr == "Level") {
		std::string string = buttonFunc.substr(levelSubstr.length(), buttonFunc.length());
		buttonFuncLevel(string);
	}
	else if (roomSubstr == "Room") {
		std::string string = buttonFunc.substr(roomSubstr.length(), buttonFunc.length());
		int value = std::stoi(string);
		buttonFuncRoom(value);
	}
}

void Room::createButtons(int room) {
	sf::Vector2f position(0, 0);
	switch (room) {
	case 1:
		position.x = ViewUtility::getViewSize().getSize().x*0.85f;
		position.y = ViewUtility::getViewSize().getSize().y*0.50f;
		addButton(DOOR_TEXTURE, "", "Room2", position, Button::ButtonType::RECTANGLE);
		position.x = ViewUtility::getViewSize().getSize().x*0.60f;
		addButton(SHOP_TEXTURE, "", "Shop", position, Button::ButtonType::RECTANGLE);
		position.x = ViewUtility::getViewSize().getSize().x*0.40f;
		addButton(DIALOGUE_TEXTURE, "", "Dialogue", position, Button::ButtonType::RECTANGLE);
		break;

	case 2:
		position.x = ViewUtility::getViewSize().getSize().x*0.22f;
		position.y = ViewUtility::getViewSize().getSize().y*0.62f;
		addButton(SOCKSHOP_TEXTURE, "", "Room1", position, Button::RECTANGLE);
		position.x = ViewUtility::getViewSize().getSize().x*0.45f;
		position.y = ViewUtility::getViewSize().getSize().y*0.645f;
		addButton(LEVEL1_TEXTURE, "", "Level01", position, Button::RECTANGLE);
		position.x = ViewUtility::getViewSize().getSize().x*0.665f;
		position.y = ViewUtility::getViewSize().getSize().y*0.585f;
		addButton(LEVEL2_TEXTURE, "", "Level02", position, Button::RECTANGLE);
		position.x = ViewUtility::getViewSize().getSize().x*0.93f;
		position.y = ViewUtility::getViewSize().getSize().y*0.445f;
		addButton(DOOR_TEXTURE, "", "Room3", position, Button::RECTANGLE);
		break;

	case 3:
		position.x = ViewUtility::getViewSize().getSize().x*0.15f;
		position.y = ViewUtility::getViewSize().getSize().y*0.50f;
		addButton(DOOR_TEXTURE, "", "Room2", position, Button::RECTANGLE);
		mLevelButtons.back()->setScale(-1.0f, 1.0f);
		break;

	default:
		Debug::log("Room index not found! Cannot load room layout.", Debug::ERRORLEVEL::FATAL);
		break;
	}
}

void Room::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, Button::ButtonType buttonType) {
	Button* button = new Button(buttonFile, "", buttonFunc, mWindow, mEventManager, pos, this, buttonType);
	button->setActive(false);
	button->setStrata(Button::THIRD);
	mGUIManager->addInterfaceElement(button);
	mLevelButtons.push_back(button);
}

void Room::buttonFuncShop() {
	mGameState->handleClicks("Shop");
}

void Room::buttonFuncDialogue() {
	mGameState->handleClicks("Dialogue");
}

void Room::buttonFuncLevel(std::string level) {
	std::string filename = LEVEL_CONFIG_PATH + "Level" + level + "Entities.json";
	GameStateLevel::getInstance().setupLevel(filename);
	GameManager::getInstance().setGameState(&GameStateLevel::getInstance());
}

void Room::buttonFuncRoom(int room) {
	GameStateMap::getInstance().changeRoom(room);
}