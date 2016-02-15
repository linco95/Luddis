#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EventManager.h"
#include "CollisionManager.h"

class EntityManager;
class GameStatePaused;
class PowerupDisplay;
class GUIManager;

class GameStateLevel : public GameState{
public:
	//TODO: have each state manage their own EntityManager, instead of the GameManager
	GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager);
	~GameStateLevel();

	void initialize(GameStatePaused* gameStatePaused);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

private:
	EntityManager* mEntityM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	EventManager mEventM;
	GameStatePaused* mGameStatePaused;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;

	PowerupDisplay* mPowerupDisplays[2];
};

#endif // !_INCLUDED_GAMESTATELEVEL_
