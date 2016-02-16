#include "Menu.h"
#include "GameStateLevel.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "GUIManager.h"
#include "GameManager.h"

static const std::string MENUBUTTON_TEXTURE = "resources/images/menubutton.png";

Menu::Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType, EntityManager* entityManager) :
mBackground(new sf::RectangleShape()),
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
	mBackground->setOutlineThickness(15.0f);
	sf::Vector2f vector2 = mWindow->getView().getSize()*(0.5f);
	setPosition(vector2);
}

void Menu::initializeButtons(MenuType menuType){
	sf::Vector2f position(getPosition());
	switch (menuType)
	{
	case Menu::MAINMENU:
		position.y -= mBackground->getSize().y / 2.5f;
		addButton(MENUBUTTON_TEXTURE, "Stara Nytt Spel", "NewGame", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Ladda Spel", "LoadGame", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Inställningar", "Settings", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Avsluta", "Quit", position);
		break;
	case Menu::PAUSEMENU:
		position.y -= mBackground->getSize().y / 2.5f;
		addButton(MENUBUTTON_TEXTURE, "Fortsätt", "Continue", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Starta Om Nivå", "ResetLevel", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Inställningar", "Settings", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Avsluta Nivå", "ExitLevel", position);
		position.y += mBackground->getSize().y / 5;
		addButton(MENUBUTTON_TEXTURE, "Avsluta Spel", "QuitGame", position);
		break;
	default:

		break;
	}
}

void Menu::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos){
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos, this);
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

void Menu::buttonFuncNewGame(){

}

void Menu::buttonFuncLoadGame(){

}

void Menu::buttonFuncContinue(){
	GameManager::getInstance().setGameState(mGameStateLevel);
	mIsAlive = false;
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