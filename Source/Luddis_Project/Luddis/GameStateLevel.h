#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "GUIManager.h"
#include "CollisionManager.h"

class EntityManager;

class GameStateLevel : public GameState{
public:
	//TODO: have each state manage their own EntityManager, instead of the GameManager
	GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GameState* currentGameState);
	~GameStateLevel();

	void update(sf::Clock& clock) override;
	void render() override;

	void onEvent(const sf::Event &aEvent) override;

private:
	EntityManager* mEM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	GameState* mCurrentGameState;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
