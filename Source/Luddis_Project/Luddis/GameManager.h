#ifndef INCLUDED_GAMEMANAGER
#define INCLUDED_GAMEMANAGER

struct GameManagerImp;

class GameManager{
public:
	static GameManager& getInstance();
	~GameManager();

	void run();

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
private:
	GameManager();

	GameManagerImp* mGMImp;
};

#endif