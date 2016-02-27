#include "Menu.h"
#include "GameStateLevel.h"
#include "GameStateMap.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "VectorMath.h"

static const std::string MENUBUTTON_TEXTURE = "Resources/Images/GUI/MenuButton.png";
static const std::string MENUBUTTON_TEXTURE_SETTINGS = "Resources/Images/GUI/ButtonSettings.png";
static const std::string MENUBUTTON_TEXTURE_RETURN = "Resources/Images/GUI/ButtonReturn.png";
static const std::string MENUBUTTON_TEXTURE_QUITGAME = "Resources/Images/GUI/ButtonQuitGame.png";
static const std::string MENUBUTTON_TEXTURE_EXITLEVEL = "Resources/Images/GUI/ButtonExitLevel.png";
static const std::string MENUBUTTON_TEXTURE_DERP = "Resources/Images/GUI/derp.png";
static const std::string MENU_BACKGROUND_TEXTURE = "Resources/Images/GUI/Filter.png";

Menu::Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType) :
mBackground(new sf::RectangleShape()),
mIsActive(false),
mIsAlive(true),
mMenuType(menuType),
mWindow(window),
mEventManager(eventManager),
mGUIManager(gUIManager){

}

Menu::~Menu(){
	internalClear();
}

void Menu::internalClear(){
	for (auto e : mButtons){
		e->kill();
	}
	delete mBackground;
}

void Menu::initialize(){
	mGameStateLevel = &GameStateLevel::getInstance();
	sf::Vector2f vector(mWindow->getView().getSize());
	vector = vector*(2.0f / 3.0f);
	mBackground->setSize(ViewUtility::getViewSize().getSize());
	mBackground->setOrigin(mBackground->getGlobalBounds().width / 2, mBackground->getGlobalBounds().height / 2);
	mBackground->setTexture(&ResourceManager::getInstance().getTexture(MENU_BACKGROUND_TEXTURE));
	mBackground->setOutlineThickness(0.0f);
	mBackground->setFillColor(sf::Color(255, 255, 255, 255));
	sf::Vector2f vector2 = mWindow->getView().getSize()*(0.5f);
	setPosition(vector2);
	initializeButtons();
}

void Menu::initializeButtons(){
	int maxButtons = 4;
	sf::Vector2f position(getPosition());
	sf::Vector2f offset(0, 0);
	switch (mMenuType)
	{
	case Menu::MAINMENU:
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Stara Nytt Spel", "NewGame", position+offset, Button::ButtonType::RECTANGLE);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Ladda Spel", "LoadGame", position + offset, Button::ButtonType::RECTANGLE);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset, Button::ButtonType::CIRCLE);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "Quit", position + offset, Button::ButtonType::CIRCLE);
		break;

	case Menu::PAUSEMENU:
		offset = {-450, 0};
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset, Button::ButtonType::CIRCLE);
		offset = { -150, 50 };
		addButton(MENUBUTTON_TEXTURE_DERP, "", "ResetLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { 150, 50 };
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset, Button::ButtonType::CIRCLE);
		offset = { 450, 0 };
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { 0, 275 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Continue", position + offset, Button::ButtonType::CIRCLE);
		break;

	case Menu::DEATHMENU:
		offset = { -200,-150 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Continue", position + offset, Button::ButtonType::CIRCLE);
		offset = { 200,-150 };
		addButton(MENUBUTTON_TEXTURE, "Starta Om Nivå", "ResetLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { -200,150 };
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { -200,150 };
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset, Button::ButtonType::CIRCLE);
		break;
	}
}

void Menu::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, Button::ButtonType buttonType){
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos, this, buttonType);
	button->setActive(true);
	mButtons.push_back(button);
	mGUIManager->addInterfaceElement(button);
}

void Menu::tick(const sf::Time& deltaTime){
	
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(*mBackground, states);
}

Menu::RenderLayer Menu::getRenderLayer() const{
	return BACKGROUND;
}

bool Menu::isAlive() const{
	return mIsAlive;
}

bool Menu::isActive() const{
	return mIsActive;
}

void Menu::setActive(const bool& active){
	mIsActive = active;
}

void Menu::onClick(std::string buttonFunc){
	if (buttonFunc == "NewGame"){
		buttonFuncNewGame();
	}
	else if (buttonFunc == "LoadGame"){
		buttonFuncLoadGame();
	}
	else if (buttonFunc == "Continue"){
		buttonFuncContinue();
	}
	else if (buttonFunc == "ExitLevel"){
		buttonFuncExitLevel();
	}
	else if (buttonFunc == "Settings"){
		buttonFuncSettings();
	}
	else if (buttonFunc == "QuitGame"){
		buttonFuncQuitGame();
	}
	else if (buttonFunc == "ResetLevel"){
		buttonFuncResetLevel();
	}
}

Menu::MenuType Menu::getMenuType() const {
	return mMenuType;
}

void Menu::kill() {
	mIsAlive = false;
}

void Menu::buttonFuncNewGame(){

}

void Menu::buttonFuncLoadGame(){

}

void Menu::buttonFuncContinue(){
	if (mGameStateLevel->playable()) {

		GameManager::getInstance().setGameState(mGameStateLevel);
		mIsAlive = false;
	}
	else {
		//TODO: Implement restarting from a checkpoint.

	}
}

void Menu::buttonFuncExitLevel(){
	GameStateLevel::getInstance().resetInventory();
	GameManager::getInstance().setGameState(&GameStateMap::getInstance());
	mIsAlive = false;
}

void Menu::buttonFuncSettings(){
	//TODO: Implement functionality
}

void Menu::buttonFuncQuitGame(){
	mWindow->close();
	mIsAlive = false;
}

void Menu::buttonFuncResetLevel(){
	mGameStateLevel->resetLevel();
	GameManager::getInstance().setGameState(mGameStateLevel);
}