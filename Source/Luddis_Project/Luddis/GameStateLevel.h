#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include <rapidjson\document.h>

class EntityManager;
class GameStatePaused;
class PowerupDisplay;
class GUIManager;
class Level;
class Spider;

class GameStateLevel : public GameState{
public:
	GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager);
	~GameStateLevel();

	void initialize(GameStatePaused* gameStatePaused);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

	void createDialogue(std::string dialogueFile);
	bool getInDialogue() const;
	void setInDialogue(bool inDialogue);
	void setupLevel(std::string levelFile);
	void resetLevel();
	void resetInventory();

	void setupMission(const std::string& mapFilename, const std::string& jsonFilename);

private:
	void readSetupFiles(const std::string& filename);

	EntityManager* mEntityM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	EventManager mEventM;
	GameStatePaused* mGameStatePaused;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Level* mLevel;
	Spider* mSpider;
	bool mInDialogue;
	bool mResetView;
	bool mSetupLevel;
	std::string mCurrentLevelFile;
	int outputTime;
	float timePassed;
	PowerupDisplay* mPowerupDisplays[2];
	struct CurrentInventory{
		int dust;
		int chips;
		int eggs;
	};
	CurrentInventory mInv;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
