#ifndef _INCLUDED_GAMESTATESTART
#define _INCLUDED_GAMESTATESTART

#include "GameState.h"
#include "EventManager.h"
#include "GUIManager.h"
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class Menu;
class Mannequin;
class ScoreCounter;

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
	void setupFiles(std::string filename);

private:
	GameStateStart();
	struct SaveInfo {
		bool NEW;
		std::string name;
		sf::Text text;
		int currentDust, maxDust;
		int chips;
		int headAccessory;
		int tailAccessory;
		//0 is chips and 1 is dust
		ScoreCounter* scoreCounters[2];
		Mannequin* mannequin;
	} mFiles[4];

	Mannequin* mMannequin;

	int mSelectedSave;
	Menu* mMenu;
	sf::Sprite mLogo, mBackground;
	GUIManager mGUIM;
	EventManager mEventM;
	sf::RenderWindow* mWindow;
};

#endif // !_INCLUDED_GAMESTATESTART
