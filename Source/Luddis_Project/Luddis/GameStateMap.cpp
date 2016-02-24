#include "GameStateMap.h"
#include <SFML/Window/Event.hpp>
#include "ViewUtility.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "GameManager.h"

static const int MAXROOMS = 2;
static const std::string BACKGROUND_TEXTURE_FILENAME = "Resources/Images/Rooms/Room";

GameStateMap::GameStateMap():
mCurrentRoom(0),
mEntityM(),
mGUIM(),
mEventM(){
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
	mCurrentRoom = 1;
	for (int i = 0; i < MAXROOMS; i++) {
		Room* room = new Room( &mGUIM, BACKGROUND_TEXTURE_FILENAME + std::to_string(i + 1) + ".png", &mEventM, mWindow);
		room->createButtons(i + 1);
		mRooms.push_back(room);
		mGUIM.addInterfaceElement(room);
	}
	mRooms.at(mCurrentRoom - 1)->setActive(true);
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
	mRooms.at(mCurrentRoom - 1)->setActive(false);
	mCurrentRoom = room;
	mRooms.at(mCurrentRoom - 1)->setActive(true);
}

void GameStateMap::update(sf::Clock& clock){
	mWindow->setView(ViewUtility::getViewSize());
	//Do game logic
	mEntityM.updateEntities(clock.getElapsedTime());
	mGUIM.updateElements(clock.restart());

	//Garbage collection
	mEntityM.removeDeadEntities();
	mGUIM.removeObsoleteElements();
}

void GameStateMap::render(){
	//Draw objects
	mEntityM.renderEntities(*mWindow);
	mGUIM.renderElements(*mWindow);
}

void GameStateMap::onEvent(const sf::Event &aEvent){
	if (true){
		switch (aEvent.type){
		case (sf::Event::EventType::MouseButtonPressed) :

			break;

		case (sf::Event::EventType::KeyPressed) :

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