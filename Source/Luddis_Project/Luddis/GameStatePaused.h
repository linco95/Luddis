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
	//TODO: have each state manage their own EntityManager, instead of the GameManager
	GameStatePaused(sf::RenderWindow* window, Menu::MenuType menuType, EntityManager* entityManager, GUIManager* guiManager);
	virtual ~GameStatePaused();

	void initialize(GameStateLevel* gameStateLevel);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

private:
	EntityManager* mEntityM;
	GUIManager* mGUIM;
	EventManager mEventM;
	GameStateLevel* mGameStateLevel;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Menu mMenu;
};

#endif // !_INCLUDED_GAMESTATEPAUSED_
