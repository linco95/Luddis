#include "GameManager.h"
#include <SFML\Graphics.hpp>
#include "Luddis.h"
#include "EntityManager.h"
#include "EventObserver.h"
#include "EventManager.h"
#include <vector>

using namespace sf;

static const std::string APPNAME = "Luddis";
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const Color BGCOLOR = Color::Black;
static const std::string TEXTURE_NAME = "Resources/Images/Knight.png";
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
		mMainWindow.create(VideoMode(WIDTH, HEIGHT), APPNAME, Style::Fullscreen);
		initializeLevel(0);
		gameLoop();
	}

	void gameOver(){
		mMainWindow.close();
	}
	void initializeLevel(const int& levelIndex){
		mPlayer = new Luddis(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mPlayer);
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
		while (mMainWindow.isOpen()){

			// Handle Events         In  EventManager
			handleEvents(mMainWindow);

			// Update Entities     |
			em->updateEntities(gameClock.getElapsedTime());
			gameClock.restart();
			// Kill dead Entities  | In EntityManager
			if (!mPlayer->isAlive()){
				gameOver();
			}
			em->removeDeadEntities();
			// Render			     (In rendermanager in the future)
			render(mMainWindow);
		}
	}
	RenderWindow mMainWindow;
	Luddis *mPlayer;
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