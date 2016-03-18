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
#include "Overlay.h"
#include "SoundEngine.h"
#include <SFML/Graphics/Rect.hpp>

static const std::string DOOR_TEXTURE = "Resources/Images/Rooms/Doorknob.png";
static const std::string GET_OUT_TEXTURE = "Resources/Images/GUI/ButtonExitLevel.png";
static const std::string SHOPBUTTON_TEXTURE = "Resources/Images/Rooms/ButtonSockShop.png";
static const std::string SOCKSHOP_TEXTURE = "Resources/Images/Rooms/Shop.png";
static const std::string DIALOGUE_TEXTURE = "Resources/Images/GUI/Button.png";
static const char* OVERLAY_TEXTURE1 = "Resources/Images/Rooms/sockshop_overlay.png";
static const char* OVERLAY_TEXTURE2 = "Resources/Images/Rooms/Room1_overlay.png";
static const std::string LEVEL1_TEXTURE = "Resources/Images/Rooms/Level1.png";
static const std::string LEVEL2_TEXTURE = "Resources/Images/Rooms/Level2.png";

static const std::string DIALOGUE_PATH = "Resources/Configs/Dialogue/";

Room::Room(GUIManager* guiManager, std::string textureFilename, EventManager* eventManager, sf::RenderWindow* window, GameState* gameState) :
	mIsActive(false),
	mIsAlive(true),
	mOverlay(nullptr),
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
	if (mOverlay != nullptr)
		mOverlay->kill();
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
	if (mOverlay != nullptr)
		mOverlay->setActive(active);
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
		mGameState->handleClicks("Dialogue");
	}
	else if (levelSubstr == "Level") {
		mGameState->handleClicks(buttonFunc);
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
		position = {1700, 150};
		addButton(GET_OUT_TEXTURE, "", "Room2", position, Button::ButtonType::CIRCLE);
		mLevelButtons.back()->setStrata(InterfaceElement::SECOND);

		position = { 164, 209 };
		addButton(SHOPBUTTON_TEXTURE, "", "Shop", position, Button::ButtonType::CIRCLE);
		mLevelButtons.back()->setStrata(InterfaceElement::SECOND);

		//position.x = ViewUtility::getViewSize().getSize().x*0.40f;
		//addButton(SHOPBUTTON_TEXTURE, "", "Dialogue", position, Button::ButtonType::RECTANGLE);

		mOverlay = new Overlay(OVERLAY_TEXTURE1, InterfaceElement::Strata::THIRD);
		mOverlay->setActive(false);
		mGUIManager->addInterfaceElement(mOverlay);
		break;

	case 2:
		position.x = ViewUtility::getViewSize().getSize().x*0.223f;
		position.y = ViewUtility::getViewSize().getSize().y*0.619f;
		addButton(SOCKSHOP_TEXTURE, "", "Room1", position, Button::RECTANGLE);

		position.x = ViewUtility::getViewSize().getSize().x*0.510f;
		position.y = ViewUtility::getViewSize().getSize().y*0.646f;
		addButton(LEVEL1_TEXTURE, "", "Level01", position, Button::RECTANGLE);

		position.x = ViewUtility::getViewSize().getSize().x*0.945f;
		position.y = ViewUtility::getViewSize().getSize().y*0.405f;
		addButton(DOOR_TEXTURE, "", "Level03", position, Button::RECTANGLE);

		position.x = ViewUtility::getViewSize().getSize().x*0.752f;
		position.y = ViewUtility::getViewSize().getSize().y*0.585f;
		addButton(LEVEL2_TEXTURE, "", "Level02", position, Button::RECTANGLE);
	
		mOverlay = new Overlay(OVERLAY_TEXTURE2, InterfaceElement::Strata::THIRD);
		mOverlay->setActive(false);
		mGUIManager->addInterfaceElement(mOverlay);

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

void Room::buttonFuncRoom(int room) {
	GameStateMap::getInstance().changeRoom(room);
}