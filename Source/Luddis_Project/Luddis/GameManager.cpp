#include "GameManager.h"
#include <SFML\Graphics.hpp>
#include "Luddis.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "EventObserver.h"
#include "EventManager.h"
#include "ResourceManager.h"
//#include "SoundEngine.h"
#include "Renderer.h"
#include "SoundEngine.h"
#include "GUIManager.h"
#include "Dialogue.h"
#include "Level.h"
#include "Button.h"
#include <vector>
#include "ScoreCounter.h"
#include "ScoreGauge.h"
#include "GameStateStart.h"
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
static const char* MOUSE_IMAGE = "Resources/Images/LuddisCursor.png";

static const char* SAVEFILES = "Resources/Configs/Savefiles/Saves.json";

static const char* MASTERBANK = "Resources/AudioBanks/Build/Desktop/Master Bank.bank";
static const char* MASTER_BANK_STRINGS = "Resources/AudioBanks/Build/Desktop/Master Bank.strings.bank";
static const char* MUSIC_BANK = "Resources/AudioBanks/Build/Desktop/Music Bank.bank";
static const char* SOUND_BANK = "Resources/AudioBanks/Build/Desktop/Audio Bank.bank";

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

		//The string bank contains all paths for the events etc.
		se->loadBank(MASTERBANK);
		se->loadBank(MASTER_BANK_STRINGS);
		se->loadBank(MUSIC_BANK);
		se->loadBank(SOUND_BANK);

		//TEMP
		se->createEvent("event:/Gameplay/Luddis/Interaction/Luddis_Dust", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Luddis/Interaction/Luddis_Crumb", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Luddis/Interaction/Luddis_Hit", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Luddis/Interaction/Luddis_Shot", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Level/Wall_Hit", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Luddis/Skills/Spiderweb", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Luddis/Skills/Static", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Obstacles/Electricity", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Obstacles/Steam", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Shop/Buy", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Shop/Sock Move Down", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Shop/Sock Move Up", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Menu/Start_Game", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Menu/Button/Button_Change", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Menu/Button/Button_Click", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Menu/Button/Button_Fail", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Ambience/Fan", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Ambience/Room", SoundEngine::SOUND);
		se->createEvent("event:/Gameplay/Ambience/Shop", SoundEngine::SOUND);
	
		se->createEvent("event:/Music/Levels/Lvl1", SoundEngine::MUSIC);
		se->createEvent("event:/Music/Levels/Lvl2", SoundEngine::MUSIC);
		se->createEvent("event:/Music/Levels/Boss", SoundEngine::MUSIC);
		se->createEvent("event:/Music/Sockshop", SoundEngine::MUSIC);
		se->createEvent("event:/Music/Meny", SoundEngine::MUSIC);

		se->createEvent("snapshot:/Music/Pausemenu", SoundEngine::MUSIC);
		se->createEvent("snapshot:/Music/Dialogue", SoundEngine::MUSIC);

		mGameStateStart = &GameStateStart::getInstance();
		mGameStatePaused = &GameStatePaused::getInstance();
		mGameStateLevel = &GameStateLevel::getInstance();
		mGameStateMap = &GameStateMap::getInstance();
		mGameStatePaused->initialize(&mMainWindow, &mEntityManager, &mGUIManager);
		mGameStateLevel->initialize(&mMainWindow, &mEntityManager, &mGUIManager);
		mGameStateMap->initialize(&mMainWindow);
		mGameStateStart->initialize(&mMainWindow);
		mGameStateStart->setupFiles(SAVEFILES);
		//mGameStateLevel->setupLevel(TEST_LEVEL);
		mCurrentGameState = mGameStateStart;

		Renderer* renderer = &Renderer::getInstance();
		View mapView;
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

	GameStateStart* mGameStateStart;
	GameStateLevel* mGameStateLevel;
	GameStatePaused* mGameStatePaused;
	GameStateMap* mGameStateMap;

	GameState* mCurrentGameState;
	RenderWindow mMainWindow;

	MouseCursor mCursor;
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
