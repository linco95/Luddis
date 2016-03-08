#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "GUIManager.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <rapidjson/document.h>

class EntityManager;
class GameStatePaused;
class PowerupDisplay;
class Level;
class Spider;
class Luddis;
class ScoreGauge;

class GameStateLevel : public GameState{
public:
	~GameStateLevel();

	static GameStateLevel& getInstance();
	//Call before using
	void initialize(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;
	void handleClicks(std::string command)override;

	//Function to create a dialogue on the level.
	void createDialogue(std::string dialogueFile);
	void fuckOffSpider();
	bool getInDialogue() const;
	void setInDialogue(bool inDialogue);
	//Used to setup a new level with its initial
	//attributes. Will clear out all vectors first.
	void setupLevel(std::string levelFile);
	//Will reset all entities and non HUD GUI elements.
	void resetLevel() override;
	void resetInventory();

	void setupMission(const std::string& jsonFilename);
	bool playable() const;

private:
	GameStateLevel();
	void updateLuddGauge();

	void readSetupFiles(const std::string& filename, bool allocate = true);

	EntityManager* mEntityM;
	GUIManager* mGUIM;
	GUIManager mResettableGUI;
	CollisionManager* mCM;
	EventManager mEventM;
	GameStatePaused* mGameStatePaused;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Level* mLevel;
	Spider* mSpider;
	Luddis* mPlayer;
	ScoreGauge* mLuddGauge;
	sf::RectangleShape mCinematicBox[2];
	bool mFirstTime;
	bool mPlayable;
	bool mInDialogue;
	float mDialogueFadeTimer;
	bool mResetView;
	std::string mCurrentLevelFile;
	int mCurrentLevel;
	int outputTime;
	float timePassed;
	PowerupDisplay* mPowerupDisplays[2];
	struct CurrentInventory{
		int dust;
		int chips;
		int eggs;
	} mInv;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
