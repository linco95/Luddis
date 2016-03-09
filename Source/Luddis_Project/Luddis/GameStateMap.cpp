#include "GameStateMap.h"
#include "GameStatePaused.h"
#include "Dialogue.h"
#include "Shop.h"
#include "SockNPC.h"
#include "Mannequin.h"
#include <SFML/Window/Event.hpp>
#include "ViewUtility.h"
#include "SoundEngine.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "GameManager.h"

static const int MAXROOMS = 3;
static const float FADEMAXTIMER = 1.0f;
static const std::string BACKGROUND_TEXTURE_FILENAME = "Resources/Images/Rooms/Room";

static const std::string DIALOGUE_PATH = "Resources/Configs/Dialogue/";

GameStateMap::GameStateMap() :
	mCurrentRoom(2),
	mCurrentDialogueID(1),
	mOccupied(false),
	mSock(new SockNPC()),
	mMannequin(new Mannequin()),
	mEntityM(),
	mGUIM(),
	mEventM() {
	sf::Vector2f offset(0.0f, (float)ViewUtility::VIEW_HEIGHT / 3.0f);
	mMannequin->move(offset);
	mGUIM.addInterfaceElement(mSock);
	mGUIM.addInterfaceElement(mMannequin);
	mEventM.attatch(this, std::vector < sf::Event::EventType > { sf::Event::MouseButtonReleased, sf::Event::MouseButtonPressed, sf::Event::MouseMoved, sf::Event::KeyPressed });
}

GameStateMap::~GameStateMap() {
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
	if (mSock != nullptr)
		mSock->setActive(false);
	if (mMannequin != nullptr)
		mMannequin->setActive(false);

	mWindow = window;
	for (int i = 0; i < MAXROOMS; i++) {
		Room* room = new Room(&mGUIM, BACKGROUND_TEXTURE_FILENAME + std::to_string(i + 1) + ".png", &mEventM, mWindow, this);
		room->createButtons(i + 1);
		mRooms.push_back(room);
		mGUIM.addInterfaceElement(room);
	}
	mRooms.at(mCurrentRoom - 1)->setActive(true);
	mGameStatePaused = &GameStatePaused::getInstance();
}

void GameStateMap::changeRoom(int room) {
	if (!mOccupied) {
		mRooms.at(mCurrentRoom - 1)->setActive(false);
		mCurrentRoom = room;
		mRooms.at(mCurrentRoom - 1)->setActive(true);
	}
	if (mCurrentRoom == 1) {
		mSock->setActive(true);
		//SoundEngine::getInstance().setEventParameter("", "", 1.0f);
	}
	else {
		mSock->setActive(false);
	}
}

void GameStateMap::update(sf::Clock& clock) {

	mWindow->setView(ViewUtility::getViewSize());
	//Do game logic
	mEntityM.updateEntities(clock.getElapsedTime());
	mGUIM.updateElements(clock.restart());

	//Garbage collection
	mEntityM.removeDeadEntities();
	mGUIM.removeObsoleteElements();
}

void GameStateMap::render() {
	//Draw objects
	mEntityM.renderEntities(*mWindow);
	mGUIM.renderElements(*mWindow);
}

void GameStateMap::onEvent(const sf::Event &aEvent) {
	if (true) {
		switch (aEvent.type) {
		case sf::Event::EventType::KeyPressed:
			if (aEvent.key.code == sf::Keyboard::Escape) {
				mGameStatePaused->createMenu(Menu::ROOMMENU, this);
				mGameStatePaused->setBackgroundParameters(nullptr, &mGUIM, this);
				GameManager::getInstance().setGameState(mGameStatePaused);
			}
			break;
		}
	}
}

void GameStateMap::handleEvents() {
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)) {
		mEventM.notify(currEvent);
	}
}

void GameStateMap::handleClicks(std::string command) {
	std::string dialogueString = command.substr(0, 8);
	if (dialogueString == "Dialogue" && !mOccupied) {
		mOccupied = true;
		sf::Vector2f pos(0.0f, (float)ViewUtility::VIEW_HEIGHT);
		std::string string = DIALOGUE_PATH + "Sock" + command + std::to_string(mCurrentDialogueID) + ".json";

		Dialogue* dialogue = new Dialogue(string, mWindow, &mGUIM, &mEventM, pos, this);
		mGUIM.addInterfaceElement(dialogue);
	}
	else if (command == "Shop" && !mOccupied) {
		mOccupied = true;
		Shop* shop = new Shop(mWindow, this, &mEventM, &mGUIM);
		mGUIM.addInterfaceElement(shop);
	}
	else if (command == "DeleteDialogue") {
		mOccupied = false;
	}
	else if (command == "ShopDelete") {
		mOccupied = false;
	}
}