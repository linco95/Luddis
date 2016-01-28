#ifndef INCLUDED_GAMEMANAGER
#define INCLUDED_GAMEMANAGER

struct GameManagerImp;

namespace sf{
	class RenderWindow;
}

class GameManager{
public:
	static GameManager& getInstance();
	~GameManager();

	void run();
	void gameOver();

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
private:
	GameManager();

	GameManagerImp* mGMImp;
};

#endif