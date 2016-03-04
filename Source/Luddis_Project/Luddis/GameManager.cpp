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
#include "ScoreCounter.h"
#include "ScoreGauge.h"
#include "GameStateLevel.h"
#include "GameStatePaused.h"
#include "GameStateMap.h"
#include "ViewUtility.h"
#include "Debug.h"
#include "MouseCursor.h"

using namespace sf;

static const std::string APPNAME = "Luddis";
static const std::string ICONPATH = "resources/images/luddisicon.png";
static const float DESIRED_ASPECTRATIO = (float)ViewUtility::VIEW_WIDTH / (float)ViewUtility::VIEW_HEIGHT;
static const Color BGCOLOR = Color::Black;
static const std::string TEXTURE_CHIPSCOUNTER = "Resources/Images/GUI/HUD_Chips_Icon.png";
static const std::string TEXTURE_LUDDCOUNTER = "Resources/Images/GUI/HUD_Ludd_Icon.png";
static const std::string TEST_LEVEL = "Resources/Configs/Levels/Level01Entities.json";
static const char* MOUSE_IMAGE = "Resources/Images/LuddisCursor.png";
static const std::string FONT_NAME = "arial.ttf";

static const char* MASTER_BANK = "Resources/Audio/Musicbank/Build/Desktop/Master Bank.bank";
static const char* MASTER_BANK_STRINGS = "Resources/Audio/Musicbank/Build/Desktop/Master Bank.strings.bank";
static const char* SOUND_BANK = "Resources/Audio/Soundbank/Build/Desktop/Ljud.bank";

static const char* EVENT_MUSIC1 = "event:/MUSIK/Bana_1";
static const char* EVENT_LUDDIS = "event:/Luddis";

static const bool VSYNCENABLED = true;

struct GameManagerImp : public EventObserver {

	GameManagerImp(){
		mEventManager.attatch(this, Event::EventType::Closed);
	}

	void run(){
		initializeGame();
		gameLoop();
	}

	void gameOver(){
		mMainWindow.close();
	}

	void setGameState(GameState* gameState){
		mCurrentGameState = gameState;
	}

	// Temporary function (might keep luddis init here). Most of this should be handled in the levelmanager/level class instead
	void initializeEntities(){


		mChipsCounter = new ScoreCounter(&mMainWindow, TEXTURE_CHIPSCOUNTER, sf::Vector2f(ViewUtility::VIEW_WIDTH*0.7f, ViewUtility::VIEW_HEIGHT-60), ScoreCounter::ScoreType::CHIPS);
		mGUIManager.addInterfaceElement(mChipsCounter);

		mLuddCounter = new ScoreCounter(&mMainWindow, TEXTURE_LUDDCOUNTER, sf::Vector2f(ViewUtility::VIEW_WIDTH * 0.3f, ViewUtility::VIEW_HEIGHT - 60), ScoreCounter::ScoreType::DUST);
		mGUIManager.addInterfaceElement(mLuddCounter);		

		// Initialize the custom mouse pointer
		mCursor.initialize(MOUSE_IMAGE, mMainWindow);
		//mEntityManager.addEntity(new MouseCursor(MOUSE_IMAGE, mMainWindow));
		//mMainWindow.setMouseCursorVisible(true);
	}

	void initializeGame(){
		srand((unsigned int)time(NULL));
		initializeWindow();
		initializeEntities();
		
	}
	// http://acamara.es/blog/2012/02/keep-screen-aspect-ratio-with-different-resolutions-using-libgdx/
	void initializeWindow(){
		// Create the window
		mMainWindow.create(VideoMode(ViewUtility::VIEW_WIDTH, ViewUtility::VIEW_HEIGHT), APPNAME, Style::Fullscreen);
		mMainWindow.setVerticalSyncEnabled(VSYNCENABLED);
		sf::Vector2u vec(800, 600);
		// Set up the viewport
		/*auto actualSize = mMainWindow.getView().getSize();
		auto desiredSize = ViewUtility::getViewSize().getSize();
		sf::FloatRect viewport(0, 0, 0, 0);

		//mMainWindow.getView().setViewport()
		decltype(desiredSize) viewport(0, 0, 1, 1);
		if (actualSize.width > actualSize.height) {

		}
		if (actualSize.width < desiredSize.width) {

		}
		else if (actualSize.height < desiredSize.height) {

		}*/
		mMainWindow.setView(ViewUtility::getViewSize());


		// Set up icon
		Image icon;
		icon.loadFromFile(ICONPATH);
		sf::Vector2u iconSize(icon.getSize());
		mMainWindow.setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());

		// Temporary splash screen
		mMainWindow.setMouseCursorVisible(false);
		sf::RectangleShape splashScreen(sf::Vector2f((float)ViewUtility::VIEW_WIDTH, (float)ViewUtility::VIEW_HEIGHT));
		splashScreen.setTexture(&ResourceManager::getInstance().getTexture("Resources/Images/splash.png"));
		mMainWindow.draw(splashScreen);
		mMainWindow.display();

	}

	void onEvent(const Event& aEvent) override{
		switch(aEvent.type){
			case Event::EventType::Closed:
				gameOver();
				break;
			/*case Event::EventType::KeyPressed:
				if(aEvent)
				break;*/
			default:
				// NO-OP
				break;
		}
	}

	void handleEvents(RenderWindow& aWindow){
		Event currEvent;
		while (aWindow.pollEvent(currEvent)){
			mEventManager.notify(currEvent);
		}
	}

	void gameLoop(){
		// To avoid multiple functioncalls every iteration of gameloop
		CollisionManager* cm = &CollisionManager::getInstance();
		SoundEngine* se = &SoundEngine::getInstance();

		se->loadBank(MASTER_BANK);
		se->loadBank(MASTER_BANK_STRINGS);
		/*se->loadBank(SOUND_BANK);
		se->createEvent(EVENT_LUDDIS, SoundEngine::EventType::SOUND);
		se->playEvent(EVENT_LUDDIS);*/ //Event not found
		se->createEvent(EVENT_MUSIC1, SoundEngine::EventType::MUSIC);
		//se->playEvent(EVENT_MUSIC1); IT GETS OLD OK?!
		
		mGameStatePaused = &GameStatePaused::getInstance();
		mGameStateLevel = &GameStateLevel::getInstance();
		mGameStateMap = &GameStateMap::getInstance();
		mGameStatePaused->initialize(&mMainWindow, &mEntityManager, &mGUIManager);
		mGameStateLevel->initialize(&mMainWindow, &mEntityManager, &mGUIManager);
		mGameStateMap->initialize(&mMainWindow);
		mGameStatePaused->createMenu(Menu::MenuType::PAUSEMENU);
		mGameStateLevel->setupLevel(TEST_LEVEL);
		mCurrentGameState = mGameStateLevel;

		View mapView;
		se->setMainVolume(10);
		Clock gameClock;
		while (mMainWindow.isOpen()){
			//Update soundengine
			se->update(gameClock.getElapsedTime());

			// Handle Events
			mCurrentGameState->handleEvents();
			//handleEvents(mMainWindow);
			// Update according to the game's state
			mCurrentGameState->update(gameClock);

			// Render according to the game's state
			mMainWindow.clear();
			mCurrentGameState->render();

			// Render the mouse on top of everything, always
			mCursor.tick();
			mMainWindow.draw(mCursor);

			// Swap the buffers
			mMainWindow.display();
		}
	}
	GUIManager mGUIManager;
	EntityManager mEntityManager;
	EventManager mEventManager;

	GameStateLevel* mGameStateLevel;
	GameStatePaused* mGameStatePaused;
	GameStateMap* mGameStateMap;

	GameState* mCurrentGameState;
	RenderWindow mMainWindow;
	
	MouseCursor mCursor;
	// Needs to be moved to corresponding level later.

	ScoreCounter *mChipsCounter;
	ScoreCounter *mLuddCounter;
	ScoreGauge *mLuddGauge;
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

void GameManager::setGameState(GameState* gameState){
	mGMImp->setGameState(gameState);
}

