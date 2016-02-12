#ifndef _INCLUDED_GAMESTATEPAUSED_
#define _INCLUDED_GAMESTATEPAUSED_

#include "GameState.h"	
#include "EntityManager.h"
#include "GUIManager.h"
#include "CollisionManager.h"
#include "Menu.h"

class GameStatePaused : public GameState{
public:
	GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType);
	~GameStatePaused();

	void update(sf::Clock& clock) override;
	void render() override;

private:
	EntityManager* mEM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Menu mMenu;
};

#endif // !_INCLUDED_GAMESTATEPAUSED_
