#include "GameManager.h"
#include <SFML\Graphics.hpp>
#include "Luddis.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "EventObserver.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "GUIManager.h"
#include "Dialogue.h"
#include "Level.h"
#include "Button.h"
#include <vector>
#include "BossDishCloth.h"
#include "Silverfish.h"
#include "Dust.h"
#include "Chips.h"
#include "Spider.h"
#include "Obstacle.h"
#include "ScoreCounter.h"
#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "ViewUtility.h"

#include <iostream>

using namespace sf;

static const std::string APPNAME = "Luddis";
static const std::string ICONPATH = "resources/images/luddisicon.png";
static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const float DESIRED_ASPECTRATIO = (float)WIDTH / (float)HEIGHT;
static const Color BGCOLOR = Color::Black;
static const std::string TEXTURE_NAME = "Resources/Images/Grafik_Luddis120x80_s1d3v1.png";
static const std::string TEXTURE_CHIPSCOUNTER = "Resources/Images/HUD_Chips_Icon.png";
static const std::string TEXTURE_LUDDCOUNTER = "Resources/Images/HUD_Ludd_Icon.png";
static const std::string FONT_NAME = "arial.ttf";
static const bool VSYNCENABLED = true;

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

		mChipsCounter = new ScoreCounter(&mMainWindow, TEXTURE_CHIPSCOUNTER, sf::Vector2f(WIDTH*0.4f, HEIGHT-60), ScoreCounter::ScoreType::CHIPS);
		GUIManager::getInstance().addInterfaceElement(mChipsCounter);

		mPlayer = new Luddis(TEXTURE_NAME, &mMainWindow);
		EntityManager::getInstance().addEntity(mPlayer);
		CollisionManager::getInstance().addCollidable(mPlayer);

		mLuddCounter = new ScoreCounter(&mMainWindow, TEXTURE_LUDDCOUNTER, sf::Vector2f(WIDTH*0.6f, HEIGHT - 60), ScoreCounter::ScoreType::DUST);
		GUIManager::getInstance().addInterfaceElement(mLuddCounter);

		mMainWindow.setMouseCursorVisible(false);
		// Temporary splash screen
		sf::RectangleShape splashScreen(sf::Vector2f((float)WIDTH, (float)HEIGHT));
		splashScreen.setTexture(&ResourceManager::getInstance().getTexture("resources/images/splash.png"));
		mMainWindow.draw(splashScreen);
		mMainWindow.display();
		
		mLevel = new Level();
		mLevel->initializeLevel(mMainWindow, mPlayer);
		EntityManager::getInstance().addEntity(mLevel);
		mMainWindow.setMouseCursorVisible(true);
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

		mMainWindow.setView(ViewUtility::getViewSize());

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
				delete mCurrentGameState;
				mCurrentGameState = new GameStatePaused(&mMainWindow, Menu::PAUSEMENU);
				}
				break;
			default:
				// NO-OP
				break;
		}
	}
	


	void handleEvents(RenderWindow& aWindow){
		Event currEvent;
		static EventManager* em = &EventManager::getInstance();
		while (aWindow.pollEvent(currEvent)){
			em->notify(currEvent);
		}
	}

	void gameLoop(){
		// To avoid multiple functioncalls every iteration of gameloop
		EntityManager* em = &EntityManager::getInstance();
		CollisionManager* cm = &CollisionManager::getInstance();
		GUIManager* gm = &GUIManager::getInstance();
		SoundEngine* se = &SoundEngine::getInstance();
		
		mCurrentGameState = new GameStateLevel(&mMainWindow);

		View mapView;
		se->setMainVolume(100);
		Clock gameClock;
		while (mMainWindow.isOpen()){

			// Handle Events       
			handleEvents(mMainWindow);
			
			// Update according to the game's state
			mCurrentGameState->update(gameClock);

			// Render according to the game's state
			mMainWindow.clear();
			mCurrentGameState->render();
			mMainWindow.display();


		}
	}

	GameState* mCurrentGameState;
	RenderWindow mMainWindow;
	Luddis *mPlayer;
	
	// Needs to be moved to corresponding level later.
	Silverfish *mEnemy1;
	Silverfish *mEnemy2;
	BossDishCloth* mBoss;

	Spider *mSpider;

	Dust *mDust;
	Dust *mDust2;
	Dust *mDust3;
	Dust *mDust4;
	Chips *mChips;
	Chips *mChips2;
	Chips *mChips3;
	ScoreCounter *mChipsCounter;
	ScoreCounter *mLuddCounter;

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