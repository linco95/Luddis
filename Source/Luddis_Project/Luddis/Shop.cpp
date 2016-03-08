#include "Shop.h"
#include "GameState.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "Inventory.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

const char* BACKGROUND_TEXTURE = "Resources/Images/GUI/ShopBackground.png";
const char* BUTTON_DIRECTION = "Resources/Images/GUI/DirectionButton.png";
const char* BUTTON_SHIT = "Resources/Images/GUI/ButcheredButton.png";

const char* FONT_NAME = "Resources/Fonts/trebuc.ttf";

Shop::Shop(sf::RenderWindow* window, GameState* gameState, EventManager* eventManager, GUIManager* guiManager) :
	mWindow(window),
	mEventManager(eventManager),
	mGUIManager(guiManager),
	mGameState(gameState),
	mInventory(&Inventory::getInstance()),
	mButtons(),
	mText(std::to_string(mInventory->getChips()),
		ResourceManager::getInstance().getFont(FONT_NAME)),
	mActivePage(0),
	mMaxPage(0),
	mBackground(ResourceManager::getInstance().getTexture(BACKGROUND_TEXTURE)),
	mIsAlive(true),
	mIsActive(true) {


	sf::Vector2f origin((float)mBackground.getTextureRect().width / 2, (float)mBackground.getTextureRect().height / 2);
	mBackground.setOrigin(origin);
	mBackground.setPosition(ViewUtility::getViewSize().getCenter());
	mText.setPosition(ViewUtility::getViewSize().getCenter() - sf::Vector2f(-100.0f,  (float)mBackground.getTextureRect().height/3.1f));
	mText.setColor(sf::Color::Black);
	mText.setCharacterSize(60);
	initialize();
}

Shop::~Shop(){
	while (!mButtons.empty()) {
		mButtons.back()->kill();
		mButtons.pop_back();
	}
	mGameState->handleClicks("ShopDelete");
}

void Shop::initialize() {
	sf::Vector2f position = mBackground.getPosition();
	float offset = (float)mBackground.getTextureRect().width;
	position.x -= offset/2;
	Button* button = new Button(BUTTON_DIRECTION, "", "Page1", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	button->setScale(-1, 1);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);
	position.x += offset;
	button = new Button(BUTTON_DIRECTION, "", "Page-1", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);
	position.x -= offset / 2;
	position.y += offset / 3;
	button = new Button(BUTTON_SHIT, "Stäng", "Close", mWindow, mEventManager, position, this, Button::RECTANGLE);
	button->setActive(true);
	mGUIManager->addInterfaceElement(button);
	mButtons.push_back(button);
}

void Shop::draw(sf::RenderTarget & target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mText, states);
}

void Shop::tick(const sf::Time & deltaTime){
	mText.setString(std::to_string(mInventory->getChips()));
}

Shop::Strata Shop::getRenderLayer() const{
	return SECOND;
}

bool Shop::isAlive() const{
	return mIsAlive;
}

bool Shop::isActive() const{
	return mIsActive;
}

void Shop::setActive(const bool & active){
	mIsActive = active;
}

void Shop::onClick(std::string command){
	std::string pageSubstr = command.substr(0, 4);
	if (pageSubstr == "Page") {
		int page = std::stoi(command.substr(pageSubstr.length(), command.length()));
		buttonFuncSwitchPage(page);
	}
	else if (command == "Close") {
		mIsAlive = false;
	}
}

void Shop::buttonFuncSwitchPage(int page){
	//Outside
	if ((mActivePage + page)>mMaxPage || (mActivePage + page)<0)
		return;

	for (auto i : mItems[mActivePage])
		i->setActive(false);
	mActivePage += page;
	for (auto i : mItems[mActivePage])
		i->setActive(true);
}
