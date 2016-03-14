#include "GameStateStart.h"
#include "Menu.h"
#include "ViewUtility.h"
#include "SoundEngine.h"
#include "GameStateMap.h"
#include "GameManager.h"
#include "Mannequin.h"
#include <SFML/Window/Event.hpp>

static const char* BACKGROUNDTEXTURE = "";

GameStateStart::GameStateStart() :
	mGUIM(),
	mEventM(),
	mMannequin(new Mannequin()){
	mMannequin->scale(-1.0f, 1.0f);
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
	//mWindow->draw(mBackground);
	Renderer::getInstance().render(*mWindow);
	mGUIM.renderElements(*mWindow);
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
	if (command == "NewGame") {
		//Just a copy of ExitLevel for now
		GameManager::getInstance().setGameState(&GameStateMap::getInstance());
		SoundEngine* se = &SoundEngine::getInstance();

		se->stopEvent("event:/Music/Meny", FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
		se->playEvent("event:/Music/Sockshop");
	}
}
