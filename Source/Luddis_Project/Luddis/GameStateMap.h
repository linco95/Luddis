#ifndef _INCLUDED_GAMESTATEMAP_
#define _INCLUDED_GAMESTATEMAP_

#include "GameState.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Menu.h"
#include "Room.h"
#include <vector>
#include <SFML/Graphics/View.hpp>

class Mannequin;
class SockNPC;
class GameStatePaused;

class GameStateMap : public GameState {
public:
	GameStateMap(GameStateMap&) = delete;
	GameStateMap& operator=(GameStateMap&) = delete;
	~GameStateMap();

	static GameStateMap& getInstance();

	void initialize(sf::RenderWindow* window);
	void changeRoom(int room);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;
	void handleClicks(std::string command)override;

private:
	GameStateMap();

	typedef std::vector<Room*> RoomVector;
	RoomVector mRooms;
	int mCurrentRoom;
	bool mOccupied;
	int mCurrentDialogueID;
	GameStatePaused* mGameStatePaused;
	SockNPC* mSock;
	Mannequin* mMannequin;
	Menu* mMenu;
	EntityManager mEntityM;
	GUIManager mGUIM;
	EventManager mEventM;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATEMAP_
