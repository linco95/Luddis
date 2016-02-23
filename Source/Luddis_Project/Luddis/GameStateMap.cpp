#include "GameStateMap.h"
#include <SFML/Window/Event.hpp>
#include "ViewUtility.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "GameManager.h"

static const int MAXROOMS = 2;
static const std::string BACKGROUND_TEXTURE_FILENAME = "Resources/Images/Rooms/Room";

GameStateMap::GameStateMap():
mEntityM(),
mGUIM(),
mEventM(){
	mEventM.attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved, sf::Event::KeyPressed });
}

GameStateMap::~GameStateMap(){
	mEventM.detatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved, sf::Event::KeyPressed });
	while (!mRooms.empty()) {
		mRooms.back()->kill();
		mRooms.pop_back();
	}
}

GameStateMap& GameStateMap::getInstance() {
	static GameStateMap gs;
	return gs;
}

void GameStateMap::initialize(sf::RenderWindow* window) {
	mWindow = window;
	for (int i = 1; i <= MAXROOMS; i++) {
		sf::Vector2f position((float)(ViewUtility::VIEW_WIDTH*(i)), 0);
		Room* room = new Room(position, &mGUIM, BACKGROUND_TEXTURE_FILENAME + std::to_string(i) + ".png");
		mRooms.push_back(room);
		mGUIM.addInterfaceElement(room);
	}
}

void GameStateMap::createMenu(Menu::MenuType menuType) {
	/*if (mMenu != nullptr)
		mMenu->kill();

	mMenu = new Menu(mWindow, &mEventM, &mGUIM, menuType);
	mMenuGUIM.addInterfaceElement(mMenu);
	mMenu->setActive(true);
	mMenu->initialize();*/
}

void GameStateMap::update(sf::Clock& clock){
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