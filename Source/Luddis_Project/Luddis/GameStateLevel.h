#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EntityManager.h"
#include "GUIManager.h"

class GameStateLevel : public GameState{
public:
	GameStateLevel();
	~GameStateLevel();

	void update(sf::Clock& clock) override;
	void render(sf::RenderWindow& window) override;

private:
	EntityManager* mEM;
	GUIManager* mGUIM;

};

#endif // !_INCLUDED_GAMESTATELEVEL_
