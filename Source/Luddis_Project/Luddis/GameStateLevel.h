#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "CollisionManager.h"

class GameStateLevel : public GameState{
public:
	GameStateLevel(sf::RenderWindow* window);
	~GameStateLevel();

	void update(sf::Clock& clock) override;
	void render() override;

private:
	EntityManager* mEM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
