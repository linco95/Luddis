#include "GameManager.h"
#include <SFML\Graphics.hpp>
using namespace sf;

struct GameManagerImp{

	GameManagerImp() :
		mIsRunning(false){
	}

	void run(){
		mMainWindow.create(VideoMode(), "Luddis", Style::Fullscreen);
		mIsRunning = true;
	}


	void gameLoop(){
		while (mMainWindow.isOpen() && mIsRunning){
			mGameClock.restart();
			// Handle Events         In  EventManager
			// Update Entities     |
			// Kill dead Entities  | In EntityManager
			// Render
		}
	}
	bool mIsRunning;
	Clock mGameClock;
	RenderWindow mMainWindow;
};

GameManager::GameManager() :
	mGMImp(new GameManagerImp()){

}
GameManager::~GameManager(){
	delete mGMImp;
}

void GameManager::run(){
	mGMImp->run();
}

GameManager& GameManager::getInstance(){
	static GameManager gm;
	return gm;
}