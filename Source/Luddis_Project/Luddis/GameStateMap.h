#ifndef _INCLUDED_GAMESTATEMAP_
#define _INCLUDED_GAMESTATEMAP_

#include "GameState.h"
#include "GUIManager.h"
#include "EventManager.h"
#include "EntityManager.h"

class GameStateMap: public GameState{
public:
	GameStateMap(sf::RenderWindow* window);
	~GameStateMap();

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

private:
	EntityManager mEntityM;
	GUIManager mGUIM;
	EventManager mEventM;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATEMAP_
