#include "GameManager.h"
#include <SFML\Graphics.hpp>
#include "Luddis.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "EventObserver.h"
#include "EventManager.h"
#include "Level.h"
#include <vector>
#include "Silverfish.h"


using namespace sf;

static const std::string APPNAME = "Luddis";
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const Color BGCOLOR = Color::Black;
static const std::string TEXTURE_NAME = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";
static const std::string FONT_NAME = "arial.ttf";

/*
TODO:
Implement states
*/
struct GameManagerImp : public EventObserver {

	GameManagerImp() {
		EventManager::getInstance().attatch(this, std::vector<Event::EventType> { Event::EventType::Closed, Event::EventType::KeyPressed } );
	}

	void run(){
		initializeGame();
		gameLoop();
	}

	void gameOver(){
		mMainWindow.close();
	}
	void initializeGame(){
		mMainWindow.create(VideoMode(WIDTH, HEIGHT), APPNAME, Style::Fullscreen);
		mMainWindow.setVerticalSyncEnabled(true);

	/*	mEnemy1 = new Silverfish(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mEnemy1);
		CollisionManager::getInstance().addCollidable(mEnemy1);
		mEnemy2 = new Silverfish(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mEnemy2);
		CollisionManager::getInstance().addCollidable(mEnemy2);*/
		mPlayer = new Luddis(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mPlayer);
		CollisionManager::getInstance().addCollidable(mPlayer);
		mLevel.initializeLevel(mMainWindow, mPlayer);
	}
	void update(const Event& aEvent) override{

		switch(aEvent.type){
			case (Event::EventType::Closed):
				gameOver();
				break;
			case (Event::EventType::KeyPressed):
				if (aEvent.key.code == Keyboard::Escape){
					gameOver();
				}
				break;
			default:
				// NO-OP
				break;
		}
	}
	
	// Temporary renderfunction until we need a rendermanager, mainly to optimize rendering and reducing drawcalls
	void render(RenderWindow& aWindow){
		aWindow.clear(BGCOLOR);
		aWindow.draw(mLevel);
		for (auto e : EntityManager::getInstance().getEntities()){
			aWindow.draw(*e);
		}

		aWindow.display();
	}

	void handleEvents(RenderWindow& aWindow){
		Event currEvent;
		while (aWindow.pollEvent(currEvent)){
			EventManager::getInstance().notify(currEvent);
		}
	}

	void gameLoop(){
		Clock gameClock;
		// To avoid multiple functioncalls every iteration of gameloop
		EntityManager* em = &EntityManager::getInstance();
		CollisionManager* cm = &CollisionManager::getInstance();
		while (mMainWindow.isOpen()){

			// Handle Events         In  EventManager
			handleEvents(mMainWindow);

			// Update Entities     |
			em->updateEntities(gameClock.getElapsedTime());
			cm->detectCollisions();
			gameClock.restart();
			// Kill dead Entities  | In EntityManager
			if (!mPlayer->isAlive()){
				gameOver();
			}
			em->removeDeadEntities();

			// Render			     (In rendermanager in the future)
			//em->renderEntities(mMainWindow);
			render(mMainWindow);
		}
	}


	RenderWindow mMainWindow;
	Luddis *mPlayer;
	Silverfish *mEnemy1;
	Silverfish *mEnemy2;

	Level mLevel; //To be replaced with LevelManager with LevelVector
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

void GameManager::gameOver(){
	mGMImp->gameOver();
}
GameManager& GameManager::getInstance(){
	static GameManager gm;
	return gm;
}