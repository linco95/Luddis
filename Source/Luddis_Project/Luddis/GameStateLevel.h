#ifndef _INCLUDED_GAMESTATELEVEL_
#define _INCLUDED_GAMESTATELEVEL_

#include "GameState.h"
#include "EventManager.h"
#include "CollisionManager.h"

class EntityManager;
class GameStatePaused;
class PowerupDisplay;
class GUIManager;
class Level;

class GameStateLevel : public GameState{
public:
	GameStateLevel(sf::RenderWindow* window, EntityManager* entityManager, GUIManager* guiManager);
	~GameStateLevel();

	void initialize(GameStatePaused* gameStatePaused);

	void update(sf::Clock& clock) override;
	void render() override;
	void onEvent(const sf::Event &aEvent) override;
	void handleEvents() override;

	void createDialogue(std::string dialogueFile, sf::Vector2f pos);
	bool getInDialogue() const;
	void setInDialogue(bool inDialogue);
	void setupLevel(std::string levelFile);
	void resetLevel();
	void resetInventory();


private:
	EntityManager* mEntityM;
	GUIManager* mGUIM;
	CollisionManager* mCM;
	EventManager mEventM;
	GameStatePaused* mGameStatePaused;
	sf::View mGUIView;
	sf::View mMapView;
	sf::RenderWindow* mWindow;
	Level* mLevel;
	bool mInDialogue;
	std::string mCurrentLevelFile;

	PowerupDisplay* mPowerupDisplays[2];
	struct CurrentInventory{
		int dust;
		int chips;
		int eggs;
	};
	CurrentInventory mInv;
};

#endif // !_INCLUDED_GAMESTATELEVEL_
