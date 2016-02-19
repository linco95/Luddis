#ifndef INCLUDED_GAMEMANAGER
#define INCLUDED_GAMEMANAGER

struct GameManagerImp;

namespace sf{
	class RenderWindow;
}

class GameState;

class GameManager{
public:
	static GameManager& getInstance();
	~GameManager();

	void run();
	void gameOver();
	void setGameState(GameState* gameState);
	void getGameState(GameState* gameState);

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
private:
	GameManager();

	GameManagerImp* mGMImp;
};

#endif