#ifndef _INCLUDED_GAMESTATESTART
#define _INCLUDED_GAMESTATESTART

#include "GameState.h"
#include "EventManager.h"
#include "GUIManager.h"
#include <SFML/Graphics/Sprite.hpp>

class Menu;
class Mannequin;

class GameStateStart : public GameState {
public:
	~GameStateStart();
	GameStateStart(GameStateStart&) = delete;
	GameStateStart& operator=(GameStateStart&) = delete;

	static GameStateStart& getInstance();
	void initialize(sf::RenderWindow* window);

	virtual void update(sf::Clock & clock) override;
	virtual void render() override;
	virtual void onEvent(const sf::Event & aEvent) override;
	virtual void handleEvents() override;
	virtual void handleClicks(std::string command) override;

private:
	GameStateStart();
	Menu* mMenu;
	Mannequin* mMannequin;
	sf::Sprite mBackground;
	GUIManager mGUIM;
	EventManager mEventM;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATESTART
