#ifndef _INCLUDED_GAMESTATEPAUSED_
#define _INCLUDED_GAMESTATEPAUSED_

#include "GameState.h"
#include "GUIManager.h"
#include "CollisionManager.h"
#include "Menu.h"

class EntityManager;

class GameStatePaused : public GameState{
public:
	//TODO: have each state manage their own EntityManager, instead of the GameManager
	GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType, EntityManager* entityManager, GameState* currentGameState);
	virtual ~GameStatePaused();

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
	Menu mMenu;
};

#endif // !_INCLUDED_GAMESTATEPAUSED_
