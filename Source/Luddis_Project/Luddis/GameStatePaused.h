#ifndef _INCLUDED_GAMESTATEPAUSED_
#define _INCLUDED_GAMESTATEPAUSED_

#include "GameState.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "Menu.h"

class EntityManager;
class GameStateLevel;

class GameStatePaused : public GameState{
public:
	GameStatePaused(GameStatePaused&) = delete;
	GameStatePaused& operator=(GameStatePaused&) = delete;
	virtual ~GameStatePaused();

	//Might want to not have pause be a singleton
	//and instead create a new instance each time a
	//pause is invoked.
	static GameStatePaused& getInstance();

	void initialize(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager);
	void setBackgroundParameters(EntityManager* entityManager, GUIManager* guiManager, GameState* gameState);
	void createMenu(Menu::MenuType menuType);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

private:
	GameStatePaused();

	EntityManager* mEntityM;
	GUIManager* mBackgroundGUIM;
	GUIManager mMenuGUIM;
	EventManager mEventM;
	GameState* mLastGameState;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Menu* mMenu;
};

#endif // !_INCLUDED_GAMESTATEPAUSED_
