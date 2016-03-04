#include "GameStateMap.h"
#include "GameStatePaused.h"
#include <SFML/Window/Event.hpp>
#include "ViewUtility.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "GameManager.h"

static const int MAXROOMS = 3;
static const float FADEMAXTIMER = 1.0f;
static const std::string BACKGROUND_TEXTURE_FILENAME = "Resources/Images/Rooms/Room";

GameStateMap::GameStateMap():
mCurrentRoom(0),
mFade(false),
mFadeTimer(0),
mFadeEffect(ViewUtility::getViewSize().getSize()),
mEntityM(),
mGUIM(),
mEventM(){
	mFadeEffect.setFillColor(sf::Color(0, 0, 0, 0));
	mFadeEffect.setOutlineColor(sf::Color(0, 0, 0, 0));
	mEventM.attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved, sf::Event::KeyPressed });
}

GameStateMap::~GameStateMap(){
	while (!mRooms.empty()) {
		mRooms.back()->kill();
		mRooms.pop_back();
	}
	mGUIM.clearInterfaceElements();
	mEventM.detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved, sf::Event::KeyPressed });
}

GameStateMap& GameStateMap::getInstance() {
	static GameStateMap gs;
	return gs;
}

void GameStateMap::initialize(sf::RenderWindow* window) {
	mWindow = window;
	mCurrentRoom = 2;
	for (int i = 0; i < MAXROOMS; i++) {
		Room* room = new Room( &mGUIM, BACKGROUND_TEXTURE_FILENAME + std::to_string(i + 1) + ".png", &mEventM, mWindow);
		room->createButtons(i + 1);
		mRooms.push_back(room);
		mGUIM.addInterfaceElement(room);
	}
	mRooms.at(mCurrentRoom - 1)->setActive(true);
	mGameStatePaused = &GameStatePaused::getInstance();
}

void GameStateMap::createMenu(Menu::MenuType menuType) {
	/*if (mMenu != nullptr)
		mMenu->kill();

	mMenu = new Menu(mWindow, &mEventM, &mGUIM, menuType);
	mMenuGUIM.addInterfaceElement(mMenu);
	mMenu->setActive(true);
	mMenu->initialize();*/
}

void GameStateMap::changeRoom(int room) {
	if (mFade) {
		mFade = false;
		mRooms.at(mCurrentRoom - 1)->setActive(false);
		mCurrentRoom = room;
		mRooms.at(mCurrentRoom - 1)->setActive(true);
	}
	else{
		mRoomToBe = room;
		mFade = true;
	}
}

void GameStateMap::update(sf::Clock& clock){
	//sf::Uint8 intensity;
	sf::Color fadeColor(0, 0, 0, (unsigned)(mFadeTimer * 255.0f));
	if (mFadeTimer <= 0 && !mFade) {

		mWindow->setView(ViewUtility::getViewSize());
		//Do game logic
		mEntityM.updateEntities(clock.getElapsedTime());
		mGUIM.updateElements(clock.restart());

		//Garbage collection
		mEntityM.removeDeadEntities();
		mGUIM.removeObsoleteElements();
	}
	else if(mFade){

		mFadeTimer += clock.restart().asSeconds();
		mFadeTimer = std::min(mFadeTimer, FADEMAXTIMER);
		mFadeEffect.setFillColor(fadeColor);
		if (mFadeTimer >= FADEMAXTIMER) {
			changeRoom(mRoomToBe);
		}
	}
	else if (!mFade) {
		mFadeTimer -= clock.restart().asSeconds();
		mFadeTimer = std::max(mFadeTimer, 0.0f);
		mFadeEffect.setFillColor(fadeColor);
	}
}

void GameStateMap::render(){
	//Draw objects
	mEntityM.renderEntities(*mWindow);
	mGUIM.renderElements(*mWindow);
	mWindow->draw(mFadeEffect);
}

void GameStateMap::onEvent(const sf::Event &aEvent) {
	if (true) {
		switch (aEvent.type) {
		case sf::Event::EventType::KeyPressed:
			if (aEvent.key.code == sf::Keyboard::Escape) {
				mGameStatePaused->createMenu(Menu::PAUSEMENU);
				mGameStatePaused->setBackgroundParameters(nullptr, &mGUIM, this);
				GameManager::getInstance().setGameState(mGameStatePaused);
			}
			break;
		}
	}
}

void GameStateMap::handleEvents(){
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)){
		mEventM.notify(currEvent);
	}
}