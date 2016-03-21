#include "Menu.h"
#include "GameStateLevel.h"
#include "GameStateMap.h"
#include "ViewUtility.h"
#include "ResourceManager.h"
#include "EventManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "SoundEngine.h"
#include "VectorMath.h"

static const std::string MENUBUTTON_TEXTURE_SETTINGS = "Resources/Images/GUI/ButtonSettings.png";
static const std::string STARTMENUBUTTON_PLAY = "Resources/Images/GUI/StartmenuPlay.png";
static const std::string STARTMENUBUTTON_SETTINGS = "Resources/Images/GUI/StartmenuSettings.png";
static const std::string STARTMENUBUTTON_QUIT = "Resources/Images/GUI/StartmenuQuit.png";
static const std::string MENUBUTTON_TEXTURE_RETURN = "Resources/Images/GUI/ButtonReturn.png";
static const std::string MENUBUTTON_TEXTURE_QUITGAME = "Resources/Images/GUI/ButtonQuitGame.png";
static const std::string MENUBUTTON_TEXTURE_EXITLEVEL = "Resources/Images/GUI/ButtonExitLevel.png";
static const std::string MENUBUTTON_TEXTURE_DERP = "Resources/Images/GUI/ButtonSound.png";
static const std::string MENU_BACKGROUND_TEXTURE = "Resources/Images/GUI/Filter.png";
static const std::string MENUSLIDER = "Resources/Images/GUI/Slider.png";
static const std::string MENUSLIDER_GAUGE = "Resources/Images/GUI/LuddGaugeFrame.png";

static const std::string MENUSELECTGAME_PLAY = "Resources/Images/GUI/ButtonPlay.png";
static const std::string MENUSELECTGAME_ERASE = "Resources/Images/GUI/ButtonErase.png";
static const std::string MENUSELECTGAME_RETURN = "Resources/Images/GUI/ButtonReturnSmall.png";
static const std::string MENUSELECTGAME_FILE = "Resources/Images/GUI/ButtonSaveFile.png";

static bool CLICK = false;

Menu::Menu(sf::RenderWindow* window, EventManager* eventManager, GUIManager* gUIManager, MenuType menuType, Menu* previousMenu) :
	mBackground(nullptr),
	mIsActive(true),
	mIsAlive(true),
	mMenuType(menuType),
	mWindow(window),
	mEventManager(eventManager),
	mGUIManager(gUIManager),
	mPreviousMenu(previousMenu) {

	mEventManager->attatch(this, sf::Event::EventType::KeyPressed);

	if (mPreviousMenu != nullptr)
		mPreviousMenu->setActive(false);

	mBackground = new sf::RectangleShape();
	mBackground->setSize(ViewUtility::getViewSize().getSize());
	mBackground->setOrigin(mBackground->getGlobalBounds().width / 2, mBackground->getGlobalBounds().height / 2);
	mBackground->setTexture(&ResourceManager::getInstance().getTexture(MENU_BACKGROUND_TEXTURE));
	mBackground->setOutlineThickness(0.0f);
	mBackground->setFillColor(sf::Color(255, 255, 255, 255));
}

Menu::~Menu() {
	mEventManager->detatch(this, sf::Event::EventType::KeyPressed);
	internalClear();
}

void Menu::internalClear() {
	for (auto e : mButtons) {
		e->kill();
	}
	for (auto s : mSliders) {
		s->kill();
	}
	if (mBackground != nullptr)
		delete mBackground;
	if (mPreviousMenu != nullptr)
		mPreviousMenu->setActive(true);
}

void Menu::initialize(GameState* gameState) {
	mGameState = gameState;
	sf::Vector2f vector2 = mWindow->getView().getCenter();
	setPosition(vector2);
	initializeButtons();
}

void Menu::initializeButtons() {
	sf::Vector2f position(getPosition());
	sf::Vector2f offset(0, 0);
	float mainVolume = SoundEngine::getInstance().getMainVolume();
	float audioVolume = SoundEngine::getInstance().getSoundVolume();
	float musicVolume = SoundEngine::getInstance().getMusicVolume();
	switch (mMenuType)
	{
	case Menu::MAINMENU:
		offset = { 0, 70 };
		addButton(STARTMENUBUTTON_PLAY, "", "Play", position + offset, Button::ButtonType::RECTANGLE);
		offset = { 0, 260 };
		addButton(STARTMENUBUTTON_SETTINGS, "", "Settings", position + offset, Button::ButtonType::RECTANGLE);
		offset = { 0, 450 };
		addButton(STARTMENUBUTTON_QUIT, "", "QuitGame", position + offset, Button::ButtonType::RECTANGLE);
		break;

	case Menu::PAUSEMENU:
		offset = { -450, 0 };
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset, Button::ButtonType::CIRCLE);
		offset = { -150, 50 };
		addButton(MENUBUTTON_TEXTURE_DERP, "", "ResetLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { 150, 50 };
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset, Button::ButtonType::CIRCLE);
		offset = { 450, 0 };
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { 0, 275 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Previous", position + offset, Button::ButtonType::CIRCLE);
		break;

	case Menu::ROOMMENU:
		offset = { 150, 50 };
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset, Button::ButtonType::CIRCLE);
		offset = { -150, 50 };
		addButton(MENUBUTTON_TEXTURE_SETTINGS, "", "Settings", position + offset, Button::ButtonType::CIRCLE);
		offset = { 0, 275 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Previous", position + offset, Button::ButtonType::CIRCLE);
		break;

	case Menu::DEATHMENU:
		//offset = { -200,-150 };
		//addButton(MENUBUTTON_TEXTURE_RETURN, "", "Continue", position + offset, Button::ButtonType::CIRCLE);
		offset = { 200,-150 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "Starta Om Nivå", "ResetLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { -200, 150 };
		addButton(MENUBUTTON_TEXTURE_EXITLEVEL, "", "ExitLevel", position + offset, Button::ButtonType::CIRCLE);
		offset = { 200, 150 };
		addButton(MENUBUTTON_TEXTURE_QUITGAME, "", "QuitGame", position + offset, Button::ButtonType::CIRCLE);
		break;

	case Menu::SETTINGSMENU:
		offset = { 0,275 };
		addButton(MENUBUTTON_TEXTURE_RETURN, "", "Previous", position + offset, Button::ButtonType::CIRCLE);
		offset = { 0, -400 };
		addSlider(MENUSLIDER, MENUSLIDER_GAUGE, "Huvud Volym : ", mainVolume, "MainV", position + offset);
		offset = { 0, -250 };
		addSlider(MENUSLIDER, MENUSLIDER_GAUGE, "Musik Volym : ", musicVolume, "Music", position + offset);
		offset = { 0, -100 };
		addSlider(MENUSLIDER, MENUSLIDER_GAUGE, "Ljud Volym : ", audioVolume, "Audio", position + offset);
		break;

	case Menu::SAVEFILESMENU:
		offset = { 1600, 300 };
		addButton(MENUSELECTGAME_PLAY, "", "NewGame", offset, Button::ButtonType::CIRCLE);

		offset = { 1600, 550 };
		addButton(MENUSELECTGAME_ERASE, "", "EraseSave", offset, Button::ButtonType::CIRCLE);

		offset = { 1600, 800 };
		addButton(MENUSELECTGAME_RETURN, "", "Previous", offset, Button::ButtonType::CIRCLE);

		offset = { 350, 200 };
		addButton(MENUSELECTGAME_FILE, "", "File0", offset, Button::ButtonType::RECTANGLE);
		mButtons.back()->setStrata(InterfaceElement::THIRD);

		offset = { 350, 425 };
		addButton(MENUSELECTGAME_FILE, "", "File1", offset, Button::ButtonType::RECTANGLE);
		mButtons.back()->setStrata(InterfaceElement::THIRD);

		offset = { 350, 650 };
		addButton(MENUSELECTGAME_FILE, "", "File2", offset, Button::ButtonType::RECTANGLE);
		mButtons.back()->setStrata(InterfaceElement::THIRD);

		offset = { 350, 875 };
		addButton(MENUSELECTGAME_FILE, "", "File3", offset, Button::ButtonType::RECTANGLE);
		mButtons.back()->setStrata(InterfaceElement::THIRD);
		break;

	case Menu::CONFIRMMENU:
		offset = { 640, 540 };
		addButton(MENUSELECTGAME_PLAY, "", "ConfirmYes", offset, Button::ButtonType::CIRCLE);
		offset = { 1280, 540 };
		addButton(MENUSELECTGAME_RETURN, "", "ConfirmNo", offset, Button::ButtonType::CIRCLE);
		break;
	}
}

void Menu::addButton(std::string buttonFile, std::string buttonText, std::string buttonFunc, sf::Vector2f pos, Button::ButtonType buttonType) {
	Button* button = new Button(buttonFile, buttonText, buttonFunc, mWindow, mEventManager, pos, this, buttonType);
	button->setActive(true);
	mButtons.push_back(button);
	mGUIManager->addInterfaceElement(button);
}

void Menu::addSlider(std::string sliderFile, std::string gaugeFile, std::string attribute, float percent, std::string buttonFunc, sf::Vector2f pos) {
	Slider* slider = new Slider(gaugeFile, sliderFile, percent, attribute, buttonFunc, pos, mWindow, mEventManager, this);
	slider->setActive(true);
	mSliders.push_back(slider);
	mGUIManager->addInterfaceElement(slider);
}

void Menu::tick(const sf::Time& deltaTime) {
	CLICK = false;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (mBackground != nullptr) {
		states.transform *= getTransform();
		target.draw(*mBackground, states);
	}
}

Menu::Strata Menu::getRenderLayer() const {
	return FIFTH;
}

bool Menu::isAlive() const {
	return mIsAlive;
}

bool Menu::isActive() const {
	return mIsActive;
}

void Menu::setActive(const bool& active) {
	mIsActive = active;
	for (auto b : mButtons) {
		b->setActive(active);
	}
	for (auto s : mSliders) {
		s->setActive(active);
	}
}

void Menu::onClick(std::string buttonFunc) {
	std::string soundSubstrs = buttonFunc.substr(0, 5);
	std::string fileSubstr = buttonFunc.substr(0, 4);

	if (buttonFunc == "NewGame") {
		mGameState->handleClicks(buttonFunc);
	}
	else if (buttonFunc == "Play") {
		mGameState->handleClicks(buttonFunc);
		buttonFuncPlay();
	}
	else if (buttonFunc == "LoadGame") {
		buttonFuncLoadGame();
	}
	else if (buttonFunc == "EraseSave") {
		buttonFuncEraseSave();
	}
	else if (buttonFunc == "Previous") {
		mGameState->handleClicks(buttonFunc);
		buttonFuncPrevious();
	}
	else if (buttonFunc == "ExitLevel") {
		buttonFuncExitLevel();
	}
	else if (buttonFunc == "Settings") {
		buttonFuncSettings();
	}
	else if (buttonFunc == "QuitGame") {
		buttonFuncQuitGame();
	}
	else if (buttonFunc == "ResetLevel") {
		buttonFuncResetLevel();
	}
	else if (soundSubstrs == "MainV") {
		std::string volumeString = buttonFunc.substr(5, buttonFunc.size());
		float volume = std::stof(volumeString);
		SoundEngine::getInstance().setMainVolume(volume);
	}
	else if (soundSubstrs == "Audio") {
		std::string volumeString = buttonFunc.substr(5, buttonFunc.size());
		float volume = std::stof(volumeString);
		SoundEngine::getInstance().setSoundVolume(volume);
	}
	else if (soundSubstrs == "Music") {
		std::string volumeString = buttonFunc.substr(5, buttonFunc.size());
		float volume = std::stof(volumeString);
		SoundEngine::getInstance().setMusicVolume(volume);
	}
	else if (fileSubstr == "File") {
		mGameState->handleClicks(buttonFunc);
	}
	else if (buttonFunc == "ConfirmYes") {
		mGameState->handleClicks("ConfirmYes");
		mIsAlive = false;
	}
	else if (buttonFunc == "ConfirmNo") {
		mGameState->handleClicks("ConfirmNo");
		mIsAlive = false;
	}
}

Menu::MenuType Menu::getMenuType() const {
	return mMenuType;
}

void Menu::kill() {
	mIsAlive = false;
}

void Menu::buttonFuncNewGame() {
	mIsAlive = false;
	GameManager::getInstance().setGameState(&GameStateMap::getInstance());
	SoundEngine::getInstance().stopEvent("event:/Music/Meny", FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void Menu::buttonFuncPlay() {
	setActive(false);
	Menu* menu = new Menu(mWindow, mEventManager, mGUIManager, SAVEFILESMENU, this);
	menu->initialize(mGameState);
	mGUIManager->addInterfaceElement(menu);
}

void Menu::buttonFuncLoadGame() {

}

void Menu::buttonFuncEraseSave() {
	//This functionality should be in a private function called createSubmenu
	//as it is repeated more than once.
	setActive(false);
	Menu* menu = new Menu(mWindow, mEventManager, mGUIManager, CONFIRMMENU, this);
	menu->initialize(mGameState);
	mGUIManager->addInterfaceElement(menu);
}

void Menu::buttonFuncPrevious() {
	if (mMenuType != MAINMENU)
		mIsAlive = false;
	if (mPreviousMenu == nullptr) {
		SoundEngine::getInstance().stopEvent("snapshot:/Music/Pausemenu");

		GameManager::getInstance().setGameState(mGameState);
	}
}

void Menu::buttonFuncExitLevel() {
	GameStateLevel::getInstance().resetInventory();
	GameManager::getInstance().setGameState(&GameStateMap::getInstance());
	SoundEngine* se = &SoundEngine::getInstance();
	SoundEngine::getInstance().stopEvent("snapshot:/Music/Pausemenu");
	std::string musicEvent = "event:/Music/Levels/Lvl" + std::to_string(GameStateLevel::getInstance().getCurrentLevel());
	se->stopEvent(musicEvent.c_str(), FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
	se->playEvent("event:/Music/Sockshop");
	mIsAlive = false;
}

void Menu::buttonFuncSettings() {
	setActive(false);
	Menu* menu = new Menu(mWindow, mEventManager, mGUIManager, SETTINGSMENU, this);
	menu->initialize(mGameState);
	mGUIManager->addInterfaceElement(menu);
}

void Menu::buttonFuncQuitGame() {
	mWindow->close();
	mIsAlive = false;
}

void Menu::buttonFuncResetLevel() {
	SoundEngine::getInstance().stopEvent("snapshot:/Music/Pausemenu");
	mGameState->resetLevel();
	GameManager::getInstance().setGameState(mGameState);
}

void Menu::onEvent(const sf::Event & aEvent) {
	if (!CLICK && mIsActive) {
		switch (aEvent.type) {
		case sf::Event::EventType::KeyPressed:
			if (aEvent.key.code == sf::Keyboard::Escape) {
				CLICK = true;
				onClick("Previous");
			}
			break;
		}
	}
}
