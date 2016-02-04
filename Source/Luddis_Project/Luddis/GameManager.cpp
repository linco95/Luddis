#include "GameManager.h"
#include <SFML\Graphics.hpp>
#include "Luddis.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "EventObserver.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "Dialogue.h"
#include "Level.h"
#include "Button.h"
#include <vector>
#include "BossDishCloth.h"
#include "Silverfish.h"
#include "Dust.h"
#include "Chips.h"
#include "Obstacle.h"
#include "ScoreCounter.h"


#include <iostream>

using namespace sf;

static const std::string APPNAME = "Luddis";
static const std::string ICONPATH = "resources/images/luddisicon.png";
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const float DESIRED_ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
static const Color BGCOLOR = Color::Black;
static const std::string TEXTURE_NAME = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";
static const std::string TEXTURE_SILVERFISH = "Resources/Images/Grafik_silverfisk_prototyp_s1d3v2.png";
static const std::string TEXTURE_DUST = "Resources/Images/Grafik_damm1_s1d4v1.png";
static const std::string TEXTURE_CHIPS = "Resources/Images/Grafik_smula2_s1d4v1.png";
static const std::string TEXTURE_BUTTON = "Resources/Images/Button";
static const std::string TEXTURE_CHIPSCOUNTER = "Resources/Images/ChipsCounter.png";
static const std::string TEXTURE_LUDDCOUNTER = "Resources/Images/LuddCounter.png";
static const std::string FONT_NAME = "arial.ttf";
static const bool VSYNCENABLED = true;

static const std::string FUCKING_ESSAY = "I just want a long string to see how sf::Text objects act with things like wrapping words etc around the screen so I just need to have thing long text. Anyways I am running out of things to say so I'm just going to leave it at this. Hopefully its long enough to satisfy my needs. Also thats what she said.";

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

	// Temporary function (might keep luddis init here). Most of this should be handled in the levelmanager/level class instead
	void initializeEntities(){


		Obstacle* mStopp = new Obstacle(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mStopp);
		CollisionManager::getInstance().addCollidable(mStopp);

		/*mEnemy1 = new Silverfish(TEXTURE_SILVERFISH, &mMainWindow);
		EntityManager::getInstance().addEntity(mEnemy1);
		CollisionManager::getInstance().addCollidable(mEnemy1);
		mEnemy2 = new Silverfish(TEXTURE_SILVERFISH, &mMainWindow);
		EntityManager::getInstance().addEntity(mEnemy2);
		CollisionManager::getInstance().addCollidable(mEnemy2);*/

		mBoss = new BossDishCloth(&mMainWindow);
		EntityManager::getInstance().addEntity(mBoss);
		CollisionManager::getInstance().addCollidable(mBoss);

		mDust = new Dust(TEXTURE_DUST, &mMainWindow);
		EntityManager::getInstance().addEntity(mDust);
		CollisionManager::getInstance().addCollidable(mDust);
		mDust2 = new Dust(TEXTURE_DUST, &mMainWindow);
		EntityManager::getInstance().addEntity(mDust2);
		CollisionManager::getInstance().addCollidable(mDust2);
		mDust3 = new Dust(TEXTURE_DUST, &mMainWindow);
		EntityManager::getInstance().addEntity(mDust3);
		CollisionManager::getInstance().addCollidable(mDust3);
		mDust4 = new Dust(TEXTURE_DUST, &mMainWindow);
		EntityManager::getInstance().addEntity(mDust4);
		CollisionManager::getInstance().addCollidable(mDust4);

		mChips = new Chips(TEXTURE_CHIPS, &mMainWindow);
		EntityManager::getInstance().addEntity(mChips);
		CollisionManager::getInstance().addCollidable(mChips);
		mChips2 = new Chips(TEXTURE_CHIPS, &mMainWindow);
		EntityManager::getInstance().addEntity(mChips2);
		CollisionManager::getInstance().addCollidable(mChips2);
		mChips3 = new Chips(TEXTURE_CHIPS, &mMainWindow);
		EntityManager::getInstance().addEntity(mChips3);
		CollisionManager::getInstance().addCollidable(mChips3);

		mChipsCounter = new ScoreCounter(&mMainWindow, TEXTURE_CHIPSCOUNTER, sf::Vector2i(400, 50), ScoreCounter::ScoreType::CHIPS);
		EntityManager::getInstance().addEntity(mChipsCounter);

		mPlayer = new Luddis(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mPlayer);
		CollisionManager::getInstance().addCollidable(mPlayer);

		mLuddCounter = new ScoreCounter(&mMainWindow, TEXTURE_LUDDCOUNTER, sf::Vector2i(550, 50), ScoreCounter::ScoreType::DUST);
		EntityManager::getInstance().addEntity(mLuddCounter);

		mDialogue = new Dialogue(FUCKING_ESSAY, &mMainWindow);
		EntityManager::getInstance().addEntity(mDialogue);

		mLevel = new Level();
		mLevel->initializeLevel(mMainWindow, mPlayer);
		EntityManager::getInstance().addEntity(mLevel);
	}

	void initializeGame(){
		srand((unsigned int)time(NULL));
		initializeWindow();
		initializeEntities();
		
	}
	// http://acamara.es/blog/2012/02/keep-screen-aspect-ratio-with-different-resolutions-using-libgdx/
	void initializeWindow(){
		// Create the window
		mMainWindow.create(VideoMode(WIDTH, HEIGHT), APPNAME, Style::Fullscreen);
		mMainWindow.setVerticalSyncEnabled(VSYNCENABLED);
		
		// Set up view (Scale to screen size)
		View view(FloatRect(0, 0, (float)WIDTH, (float)HEIGHT));
		mMainWindow.setView(view);

		// Set up icon
		Image icon;
		icon.loadFromFile(ICONPATH);
		sf::Vector2u iconSize(icon.getSize());
		mMainWindow.setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());

		
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
	


	void handleEvents(RenderWindow& aWindow){
		Event currEvent;
		while (aWindow.pollEvent(currEvent)){
			EventManager::getInstance().notify(currEvent);
		}
	}

	void gameLoop(){
		// To avoid multiple functioncalls every iteration of gameloop
		EntityManager* em = &EntityManager::getInstance();
		CollisionManager* cm = &CollisionManager::getInstance();
		SoundEngine* se = &SoundEngine::getInstance();
		se->setMainVolume(10);
		Clock gameClock;
		while (mMainWindow.isOpen()){

			// Handle Events       
			handleEvents(mMainWindow);

			// Update Entities     |
			se->update(gameClock.getElapsedTime());
			em->updateEntities(gameClock.restart());
			cm->detectCollisions();

			
			// Kill dead Entities  
			if (!mPlayer->isAlive()){
				gameOver();
			}
			cm->removeDeadCollidables();
			em->removeDeadEntities();

			// Render			    
			em->renderEntities(mMainWindow);
			
		}
	}


	RenderWindow mMainWindow;
	Luddis *mPlayer;
	
	// Needs to be moved to corresponding level later.
	Silverfish *mEnemy1;
	Silverfish *mEnemy2;
	BossDishCloth* mBoss;

	Dust *mDust;
	Dust *mDust2;
	Dust *mDust3;
	Dust *mDust4;
	Chips *mChips;
	Chips *mChips2;
	Chips *mChips3;
	ScoreCounter *mChipsCounter;
	ScoreCounter *mLuddCounter;
	Dialogue* mDialogue;

	Level* mLevel; //To be replaced with LevelManager with LevelVector
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