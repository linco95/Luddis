#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "GUIManager.h"
#include "CollisionManager.h"

class EntityManager;
class EventManager;
class GameStatePaused;

class GameStateLevel : public GameState{
public:
	//TODO: have each state manage their own EntityManager, instead of the GameManager
	GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager);
	~GameStateLevel();

	void initialize(GameStatePaused* gameStatePaused);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;

private:
	EntityManager* mEntityM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	EventManager* mEventM;
	GameStatePaused* mGameStatePaused;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
