#include "Level.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "Silverfish.h"
#include "BossDishCloth.h"
#include "BackgroundEffect.h"
#include "Luddis.h"
#include "EventZone.h"
#include "Chips.h"
#include "Dust.h"
#include "Obstacle.h"
#include "SpiderEgg.h"
#include "TutorialText.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "GameStateLevel.h"
#include <SFML\Audio.hpp>
#include <array>
#include <map>
#include <iostream>

using namespace sf;

static const float X_OFFSET = 200.f,
				   Y_OFFSET = 50.f,
				   SCROLLSPEED = 200;

static const std::array<std::string, 5> CONFIGMEMBERS = { "Background", "Silverfish_spawns", "Obstacle_spawns", "Boss_spawns", "Boss_config" };
static const char* mapfilepath = "Resources/Configs/Levels/Level01MAP.png";
static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;

static const std::string EFFECT_FILEPATH = "Resources/Images/Placeholder_BackgroundEffect.png";
static const float EFFECT_LIFETIME = 18.5f;
static const float EFFECT_SPEED = 150;
static const sf::Vector2f vec(-1, 1);
static const float EFFECT_INTERVAL = 3.5f;

// Temporary, needs to be a bit more dynamic
enum EntityType{
	CHIPS,
	DUST,
	EGG
};
struct spawnableInfo{
	spawnableInfo(const sf::Color& aColor, const EntityType& aType) :
		color(aColor),
		type(aType){}
	sf::Color color;
	EntityType type;
};
spawnableInfo spawns[] {
		spawnableInfo(sf::Color(0, 0, 0), CHIPS),
		spawnableInfo(sf::Color(255, 0, 0), DUST),
		spawnableInfo(sf::Color(0, 0, 255), EGG)};
Level::Level(EntityManager* entityManager, GameStateLevel* gameStateLevel) :
mIsActive(true),
mEntityManager(entityManager),
mGameStateLevel(gameStateLevel),
mEffectInterval(EFFECT_INTERVAL)
{

}
Level::~Level(){
	
}

#include "Debug.h"
//Could this be remade to return a vector of entities,
//so it doesnt have to be run in realtime?
void Level::readInitMap(){
	EntityManager::EntitiesVector entities;
	sf::Image mapImage;
	bool loaded = mapImage.loadFromFile(mapfilepath);
	assert(loaded);

	// pixelArray is a c-style array of all pixels in the image
	const sf::Uint8 *pixelArray = mapImage.getPixelsPtr();
	const std::size_t arraySize = mapImage.getSize().x * mapImage.getSize().y * 4;

	// For every pixel in the image
	for (std::size_t i = 0; i < arraySize; i += 4){
		sf::Color pixel = { *(pixelArray + i), *(pixelArray + i + 1), *(pixelArray + i + 2), *(pixelArray + i + 3) };
		// Check to see if the color is associated with a collectible
		for (std::size_t j = 0; j < sizeof(spawns) / sizeof(spawnableInfo); j++){
			if (spawns[j].color == pixel){
				CollidableEntity* obj = 0;
				// Calculate the pixels position on level
				sf::Vector2<std::size_t> pixPos;
				pixPos.x = (i / 4) % mapImage.getSize().x;
				pixPos.y = i / (4 * mapImage.getSize().x);

				// Check which type should be spawned, and create it
				Debug::log("Spawning " + std::to_string(spawns[j].type) + " at " + std::to_string(pixPos.x) + ", " + std::to_string(pixPos.y), Debug::INFO);
				switch (spawns[j].type){
				case DUST:
					obj = new Dust(mWindow, "Resources/Images/Grafik_damm4_s1d4v1.png", sf::Vector2f((float)pixPos.x, (float)pixPos.y), 0);
					break;
				case CHIPS:
					obj = new Chips(mWindow, "Resources/Images/Grafik_smula1_s1d4v1.png", sf::Vector2f((float)pixPos.x, (float)pixPos.y), 0);
					break;
				case EGG:
					obj = new SpiderEgg(mWindow, "Resources/Images/Grafik_TrasanProjektil_S2D5V1.png", sf::Vector2f((float)pixPos.x, (float)pixPos.y));
					break;
				default:
					Debug::log("Color not defined [" + std::to_string(pixel.r) + ", " + std::to_string(pixel.g) + ", " + std::to_string(pixel.b) + "]", Debug::WARNING);
					break;
				}
				// If an object was created (should always be true) add the collidableentity to the entitymanager and collisionmanager
				if (obj != 0){
					entities.push_back(obj);
					mEntityManager->addEntity(obj);
					CollisionManager::getInstance().addCollidable(obj);
				}
				break;
			}
		}
	}
}

void Level::initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc){
	CollisionManager* cm = &CollisionManager::getInstance();
	ResourceManager* rm = &ResourceManager::getInstance();

	for (std::size_t i = 0; i < CONFIGMEMBERS.size(); i++){
		//Is this really optimal? Some levels will have enemies
		//that are not present on other levels.
		std::cout << CONFIGMEMBERS[i] << std::endl;
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}
	assert(configDoc.HasMember("Level"));
	int level = configDoc["Level"].GetInt();

	// Silverfishes
	const rapidjson::Value& fishSpawns = configDoc["Silverfish_spawns"];
	assert(fishSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = fishSpawns.Begin(); itr != fishSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());
		assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());

		float x = (float)(*itr)["x"].GetInt();
		float y = (float)(*itr)["y"].GetInt();
		sf::Vector2f pos(x, y);
		float angle = (float)(*itr)["angle"].GetDouble();
		float act = (float)(*itr)["activationpos"].GetInt();

		Silverfish* fish = new Silverfish(mWindow, pos, angle, act, mTarget);
		mEntityManager->addEntity(fish);
		cm->addCollidable(fish);
		std::cout << x << " " << y << std::endl;
	}

	//TutorialTexts
	if (configDoc.HasMember("TutorialText_spawns") && configDoc["TutorialText_spawns"].IsArray()){
		const rapidjson::Value& tutorialSpawns = configDoc["TutorialText_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = tutorialSpawns.Begin(); itr != tutorialSpawns.End(); itr++){
			assert(itr->IsObject());
			assert(itr->HasMember("x") && (*itr)["x"].IsInt());
			assert(itr->HasMember("y") && (*itr)["y"].IsInt());
			assert(itr->HasMember("text") && (*itr)["text"].IsString());
			assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());
			float x = (float)(*itr)["x"].GetInt();
			float y = (float)(*itr)["y"].GetInt();
			sf::Vector2f pos(x, y);
			std::string text = (*itr)["text"].GetString();
			float act = (float)(*itr)["activationpos"].GetInt();

			TutorialText* tut = new TutorialText(mWindow, pos, act, text, mTarget);
			mEntityManager->addEntity(tut);
			std::cout << x << " " << y << std::endl;
		}
	}

	// Obstacles
	const rapidjson::Value& obstacleSpawns = configDoc["Obstacle_spawns"];
	assert(obstacleSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = obstacleSpawns.Begin(); itr != obstacleSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("type") && (*itr)["type"].IsInt());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());
		assert(itr->HasMember("height") && (*itr)["height"].IsInt());
		assert(itr->HasMember("width") && (*itr)["width"].IsInt());
		assert(itr->HasMember("debug") && (*itr)["debug"].IsInt());

		int type =(*itr)["type"].GetInt();
		//Position
		float x = (float)(*itr)["x"].GetInt();
		float y = (float)(*itr)["y"].GetInt();
		sf::Vector2f pos(x, y);

		float angle = (float)(*itr)["angle"].GetDouble();
		// Size
		float sX = (float)(*itr)["width"].GetInt();
		float sY = (float)(*itr)["height"].GetInt();
		sf::Vector2f size(sX, sY);
		// Debug (draw) mode
		bool debug = false;
		if ((int)(*itr)["debug"].GetInt() == 1){
			debug = true;
		}

		Obstacle* obstacle = new Obstacle(mWindow, Obstacle::ObstacleType(type),pos, angle, size, debug);
		mEntityManager->addEntity(obstacle);
		cm->addCollidable(obstacle);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	// Initialize eggs, chips and ludd from a map
	readInitMap();

	// The boss
	const rapidjson::Value& bossSpawns = configDoc["Boss_spawns"];
	assert(bossSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = bossSpawns.Begin(); itr != bossSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());

		float x = (float)(*itr)["x"].GetInt();
		float y = (float)(*itr)["y"].GetInt();
		sf::Vector2f pos(x, y);
		float act = (float)(*itr)["activationpos"].GetInt();

		BossDishCloth* boss = new BossDishCloth(mWindow, pos, act, mTarget, mEntityManager);
		mEntityManager->addEntity(boss);
		cm->addCollidable(boss);
		std::cout << x << " " << y << std::endl;
	}

	//Event zones
	if (configDoc.HasMember("EventZone_rect_spawns")){
		const rapidjson::Value& eventZoneRectSpawns = configDoc["EventZone_rect_spawns"];
		assert(eventZoneRectSpawns.IsArray());
		for (rapidjson::Value::ConstValueIterator itr = eventZoneRectSpawns.Begin(); itr != eventZoneRectSpawns.End(); itr++){
			assert(itr->IsObject());
			assert(itr->HasMember("x") && (*itr)["x"].IsInt());
			assert(itr->HasMember("y") && (*itr)["y"].IsInt());
			assert(itr->HasMember("width") && (*itr)["width"].IsInt());
			assert(itr->HasMember("height") && (*itr)["height"].IsInt());
			assert(itr->HasMember("eventtype") && (*itr)["eventtype"].IsInt());

			float x = (float)(*itr)["x"].GetInt();
			float y = (float)(*itr)["y"].GetInt();
			float width = (float)(*itr)["width"].GetInt();
			float height = (float)(*itr)["height"].GetInt();
			int eventType = (*itr)["eventtype"].GetInt();
			Vector2f size(width, height);
			Vector2f position(x, y);
			Shape* shape = new RectangleShape(size);

			EventZone* zone = new EventZone(mGameStateLevel, EventZone::EventType(eventType), position, shape, 0, level);
			cm->addCollidable(zone);
			std::cout << x << " " << y << width << " " << height << std::endl;
		}
	}

	//The background
	const rapidjson::Value& background = configDoc["Background"];
	assert(background.IsObject());
	const rapidjson::Value& segments = background["segments"];
	assert(segments.IsInt());
	assert(background["filename"].IsString());
	for (int i = 0; i < segments.GetInt(); i++){
		std::string BGFILEPATH = background["filename"].GetString();
		int position = background["filename"].GetStringLength() - 4;
		BGFILEPATH.insert(position, std::to_string(i+1));
		rm->loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), Texture::getMaximumSize())));
		sf::IntRect rect(0, 0, (int)rm->getTexture(BGFILEPATH).getSize().x, (int)rm->getTexture(BGFILEPATH).getSize().y);
		mMapBounds.height = rect.height;
		sf::Sprite sprite(rm->getTexture(BGFILEPATH));
		sprite.setPosition((float)mMapBounds.width, 0.0f);
		mBackgroundImages.push_back(sprite);
		increaseMapBounds(rect);
	}
}

void Level::increaseMapBounds(sf::IntRect size){
	mMapBounds.width += size.width;
}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget, std::string levelFilename){
	//assert(aTarget != 0);

	mTarget = aTarget;
	mWindow = &aWindow;

	std::string configText = ResourceManager::getInstance().loadJsonFile(levelFilename);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	initializeEntities(mWindow, configDoc);

	mPointsOfNoReturn.push_back(mWindow->getSize().x / 2 + 1000.f);
	mCurrentPONR = mWindow->getView().getSize().x / 2;
	SoundEngine::getInstance().playMusic("resources/music/musik16.wav");

}


void Level::tick(const sf::Time& deltaTime) {
	updateView(deltaTime);
	mEffectInterval -= deltaTime.asSeconds();
	if (mEffectInterval <= 0) {
		createEffects();
		mEffectInterval = (float)(rand() % 5);
	}
}

void Level::updateView(const Time& deltaTime){
	View view = mWindow->getView();
	float xPos = mTarget->getPosition().x;
	float yPos = mTarget->getPosition().y;
	float screenXPos = view.getCenter().x;
	float screenYPos = view.getCenter().y;
	Vector2f deltaMove(SCROLLSPEED, SCROLLSPEED);
	deltaMove *= deltaTime.asSeconds();
	float minX = mCurrentPONR;
	float maxX = mMapBounds.width - view.getSize().x / 2;
	float minY = view.getSize().y / 2;
	float maxY = mMapBounds.height - view.getSize().y / 2;
	
	if (xPos < screenXPos - X_OFFSET / 2){
		deltaMove.x *= -1;
	}
	else if (xPos < screenXPos + X_OFFSET / 2){
		deltaMove.x = 0;
	}

	if (yPos < screenYPos - Y_OFFSET / 2){
		deltaMove.y *= -1;
	}
	else if (yPos < screenYPos + Y_OFFSET / 2){
		deltaMove.y = 0;
	}

	if (screenXPos >= minX && screenXPos <= maxX) {
		if (!mPointsOfNoReturn.empty() && screenXPos >= mPointsOfNoReturn.back()){
			mCurrentPONR = mPointsOfNoReturn.back();
			mPointsOfNoReturn.pop_back();
		}
		view.move(deltaMove.x, 0);
		view.getCenter().x < minX ? view.setCenter(minX, view.getCenter().y) : 0;
		view.getCenter().x > maxX ? view.setCenter(maxX, view.getCenter().y) : 0;
	}
	if (screenYPos >= minY && screenYPos <= maxY) {
		view.move(0, deltaMove.y);
		view.getCenter().y < minY ? view.setCenter(view.getCenter().x, minY) : 0;
		view.getCenter().y > maxY ? view.setCenter(view.getCenter().x, maxY) : 0;
	}
	mWindow->setView(view);
}
bool Level::isAlive() const{
	return true;
}

bool Level::isActive() const{
	return mIsActive;
}

void Level::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Level::getRenderLayer() const {
	return LAYER;
}

void Level::draw(RenderTarget& target, RenderStates states) const {
	int max = mBackgroundImages.size();
	for (int i = 0; i < max; i++)
	{
		float x = mWindow->getView().getCenter().x;
		float xMin = (mMapBounds.width / max) * (i + 1) - mWindow->getView().getSize().x*3;
		float xMax = (mMapBounds.width / max) * (i + 1) + mWindow->getView().getSize().x*3;

		if (x < xMax && x >= xMin){
			target.draw(mBackgroundImages[i]);
		}
	}
}

void Level::createEffects() {
	//Background effect
	for (int i = 0; i <= rand() % 20; i++){
	BackgroundEffect* eff = new BackgroundEffect(EFFECT_FILEPATH, vec*EFFECT_SPEED, sf::Vector2f((float)(rand() %15000),-100) + vec*EFFECT_SPEED / 3.0f, EFFECT_LIFETIME, mTarget);
	mEntityManager->addEntity(eff);
	}
}