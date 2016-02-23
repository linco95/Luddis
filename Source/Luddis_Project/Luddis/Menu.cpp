#include "Menu.h"
#include "GameStateLevel.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "VectorMath.h"

static const std::string MENUBUTTON_TEXTURE = "Resources/Images/GUI/MenuButton.png";
static const std::string MENUBUTTON_TEXTURE_SETTINGS = "Resources/Images/GUI/ButtonSettings.png";
static const std::string MENUBUTTON_TEXTURE_RETURN = "Resources/Images/GUI/ButtonReturn.png";
static const std::string MENUBUTTON_TEXTURE_QUITGAME = "Resources/Images/GUI/ButtonQuitGame.png";
static const std::string MENUBUTTON_TEXTURE_EXITLEVEL = "Resources/Images/GUI/ButtonExitLevel.png";
static const std::string MENU_BACKGROUND_TEXTURE = "Resources/Images/GUI/MenuBackground.png";

Menu::Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType, EntityManager* entityManager) :
mBackground(new sf::RectangleShape()),
mIsActive(false),
mIsAlive(true),
mMenuType(menuType),
mWindow(window),
mEventManager(eventManager),
mEntityManager(entityManager),
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

void Menu::initialize(GameStateLevel* gameStateLevel){
	mGameStateLevel = gameStateLevel;
	sf::Vector2f vector(mWindow->getView().getSize());
	vector = vector*(2.0f / 3.0f);
	mBackground->setSize(vector);
	mBackground->setOrigin(vector.x / 2, vector.y / 2);
	mBackground->setOutlineColor(sf::Color::Magenta);
	mBackground->setOutlineThickness(5.0f);
	sf::Vector2f vector2 = mWindow->getView().getSize()*(0.5f);
	setPosition(vector2);
	initializeButtons();
}

void Menu::initializeButtons(){
	int maxButtons;
	sf::Vector2f position(getPosition());
	sf::Vector2f offset(0, mBackground->getSize().y / 2.5f);
	switch (mMenuType)
	{
	case Menu::MAINMENU:
		maxButtons = 4;
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Stara Nytt Spel", "NewGame", position+offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Ladda Spel", "LoadGame", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "Quit", position + offset);
		break;

	case Menu::PAUSEMENU:
		maxButtons = 5;
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Continue", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Starta Om Nivå", "ResetLevel", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset);
		break;

	case Menu::DEATHMENU:
		maxButtons = 4;
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Continue", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE, "Starta Om Nivå", "ResetLevel", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset);
		offset = VectorMath::rotateVector(offset, (float)(360 / maxButtons));
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset);
		break;
	}
}

void Menu::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos){
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos, this);
	button->setActive(true);
	button->setScale(0.5f, 0.5f);
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

}

void Menu::buttonFuncSettings(){

}

void Menu::buttonFuncQuitGame(){
	mWindow->close();
	mIsAlive = false;
}

void Menu::buttonFuncResetLevel(){
	mGameStateLevel->resetLevel();
	GameManager::getInstance().setGameState(mGameStateLevel);
}