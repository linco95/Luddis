#include "Level.h"
#include "SoundEngine.h"
#include "Silverfish.h"
#include "BossDishCloth.h"
#include "BackgroundEffect.h"
#include "Luddis.h"
#include "EventZone.h"
#include "Obstacle.h"
#include "TutorialText.h"
#include "ViewUtility.h"
#include "VectorMath.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "GameStateLevel.h"
#include <SFML\Audio.hpp>
#include <array>
#include <map>
#include "Debug.h"
#include "Collectible.h"

using namespace sf;

static const float X_OFFSET = 200.f,
				   Y_OFFSET = 50.f,
				   SCROLLSPEED = 200;

static const float EFFECT_INTERVAL = 0.7f,
				   EFFECT_SPEED = 400,
				   EFFECT_LIFETIME = 7.5f;

static const char* EFFECT_FILEPATH = "Resources/Images/Rag_projectile.png";
static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
static const char* mapfilepath;

Level::Level(EntityManager* entityManager) :
mIsActive(true),
mProgress(0),
mEntityManager(entityManager),
mGameStateLevel(&GameStateLevel::getInstance()),
mEffectInterval(EFFECT_INTERVAL)
{

}

Level::~Level() {
	
}

void Level::readInitMap(const std::string& filename) {
	CollisionManager* cm = &CollisionManager::getInstance();
	ResourceManager::PixelVector pixelVector = ResourceManager::getInstance().readMap(filename);
	CollidableEntity* obj = 0;

	for (auto e : pixelVector) {
		if (e.color == sf::Color(0, 0, 0)) {//Black Dust
			obj = new Collectible(mWindow, "Resources/Images/Dust.png", e.position, Collectible::CollectibleType::DUST);
				}
		else if (e.color == sf::Color(255, 255, 0)) {//Yellow Chips
			obj = new Collectible(mWindow, "Resources/Images/Chips.png", e.position, Collectible::CollectibleType::CHIPS);
				}
		else if (e.color == sf::Color(0, 0, 255)) {//Blue SpiderEgg
			obj = new Collectible(mWindow, "Resources/Images/Spritesheets/Spider_egg", e.position, Collectible::CollectibleType::SPIDEREGG);
			}
		if (obj != 0) {
			mEntityManager->addEntity(obj);
			cm->addCollidable(obj);
			obj = 0;
		}
	}
}

void Level::initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc) {
	CollisionManager* cm = &CollisionManager::getInstance();
	ResourceManager* rm = &ResourceManager::getInstance();

	int level = 0;
	if (configDoc.HasMember("Level"))
		level = configDoc["Level"].GetInt();

	// Silverfishes
	if (configDoc.HasMember("Silverfish_spawns") && configDoc["Silverfish_spawns"].IsArray()) {
	const rapidjson::Value& fishSpawns = configDoc["Silverfish_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = fishSpawns.Begin(); itr != fishSpawns.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());
		assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());
		assert(itr->HasMember("type") && (*itr)["type"].IsInt());

		float x = (float)(*itr)["x"].GetInt();
		float y = (float)(*itr)["y"].GetInt();
		sf::Vector2f pos(x, y);
		float angle = (float)(*itr)["angle"].GetDouble();
		float act = (float)(*itr)["activationpos"].GetInt();
		int type = (*itr)["type"].GetInt();

		Silverfish* fish = new Silverfish(mWindow, Silverfish::FishType(type), pos, angle, act, mTarget);
		mEntityManager->addEntity(fish);
		cm->addCollidable(fish);
		Debug::log("Spawning silverfish at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
	}
	}

	//TutorialTexts
	if (configDoc.HasMember("TutorialText_spawns") && configDoc["TutorialText_spawns"].IsArray()) {
		const rapidjson::Value& tutorialSpawns = configDoc["TutorialText_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = tutorialSpawns.Begin(); itr != tutorialSpawns.End(); itr++) {
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
			Debug::log("Spawning tutorial text at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
		}
	}

	if (configDoc.HasMember("Obstacle_spawns") && configDoc["Obstacle_spawns"].IsArray()) {
	// Obstacles
	const rapidjson::Value& obstacleSpawns = configDoc["Obstacle_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = obstacleSpawns.Begin(); itr != obstacleSpawns.End(); itr++) {
		assert(itr->IsObject());
		assert(itr->HasMember("type") && (*itr)["type"].IsInt());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());
		assert(itr->HasMember("height") && (*itr)["height"].IsInt());
		assert(itr->HasMember("width") && (*itr)["width"].IsInt());
		assert(itr->HasMember("level") && (*itr)["level"].IsInt());

		int type = (*itr)["type"].GetInt();
		int levelNumber = (*itr)["level"].GetInt();
		//Position
		float x = (float)(*itr)["x"].GetInt();
		float y = (float)(*itr)["y"].GetInt();
		sf::Vector2f pos(x, y);

		float angle = (float)(*itr)["angle"].GetDouble();
		// Size
		float sX = (float)(*itr)["width"].GetInt();
		float sY = (float)(*itr)["height"].GetInt();
		sf::Vector2f size(sX, sY);

		Obstacle* obstacle = new Obstacle(mWindow, Obstacle::ObstacleType(type), pos, angle, size, levelNumber);
		mEntityManager->addEntity(obstacle);
		cm->addCollidable(obstacle);
		Debug::log("Spawning obstacle at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
	}
	}


	// The boss
	if (configDoc.HasMember("Boss_spawns") && configDoc["Boss_spawns"].IsArray()) {
	const rapidjson::Value& bossSpawns = configDoc["Boss_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = bossSpawns.Begin(); itr != bossSpawns.End(); itr++) {
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
		Debug::log("Spawning boss at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
	}
	}

	//Event zones
	if (configDoc.HasMember("EventZone_rect_spawns")) {
		const rapidjson::Value& eventZoneRectSpawns = configDoc["EventZone_rect_spawns"];
		assert(eventZoneRectSpawns.IsArray());
		for (rapidjson::Value::ConstValueIterator itr = eventZoneRectSpawns.Begin(); itr != eventZoneRectSpawns.End(); itr++) {
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

			EventZone* zone = new EventZone(EventZone::EventType(eventType), position, shape, 0, level);
			cm->addCollidable(zone);
			Debug::log("Spawning Event zone at: [" + std::to_string(x) + ", " + std::to_string(y) + "], with the dimensions (" + std::to_string(width) + ", " + std::to_string(height) + ")", Debug::INFO);
		}
	}

	//The background
	if (configDoc.HasMember("Background")) {
	const rapidjson::Value& background = configDoc["Background"];
	assert(background.IsObject());
	const rapidjson::Value& segments = background["segments"];
	assert(segments.IsInt());
	assert(background["filename"].IsString());
		for (int i = 0; i < segments.GetInt(); i++) {
		std::string BGFILEPATH = background["filename"].GetString();
		int position = background["filename"].GetStringLength() - 4;
			BGFILEPATH.insert(position, std::to_string(i + 1));
		rm->loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), Texture::getMaximumSize())));
		sf::IntRect rect(0, 0, (int)rm->getTexture(BGFILEPATH).getSize().x, (int)rm->getTexture(BGFILEPATH).getSize().y);
		mMapBounds.height = rect.height;
		sf::Sprite sprite(rm->getTexture(BGFILEPATH));
		sprite.setPosition((float)mMapBounds.width, 0.0f);
		mBackgroundImages.push_back(sprite);
		increaseMapBounds(rect);
	}
	}
	//Dust
	if (configDoc.HasMember("Dust_spawns") && configDoc["Dust_spawns"].IsArray()) {
		const rapidjson::Value& dustSpawns = configDoc["Dust_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = dustSpawns.Begin(); itr != dustSpawns.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("x") && (*itr)["x"].IsInt());
			assert(itr->HasMember("y") && (*itr)["y"].IsInt());

			float x = (float)(*itr)["x"].GetInt();
			float y = (float)(*itr)["y"].GetInt();
			sf::Vector2f pos(x, y);

			Collectible* dust = new Collectible(mWindow, "Resources/Images/Dust.png", pos, Collectible::CollectibleType::DUST);
			mEntityManager->addEntity(dust);
			cm->addCollidable(dust);
			Debug::log("Spawning dust at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
		}
	}
	//Chips
	if (configDoc.HasMember("Chips_spawns") && configDoc["Chips_spawns"].IsArray()) {
		const rapidjson::Value& chipsSpawns = configDoc["Chips_spawns"];
		for (rapidjson::Value::ConstValueIterator itr = chipsSpawns.Begin(); itr != chipsSpawns.End(); itr++) {
			assert(itr->IsObject());
			assert(itr->HasMember("x") && (*itr)["x"].IsInt());
			assert(itr->HasMember("y") && (*itr)["y"].IsInt());

			float x = (float)(*itr)["x"].GetInt();
			float y = (float)(*itr)["y"].GetInt();
			sf::Vector2f pos(x, y);

			Collectible* chips = new Collectible(mWindow, "Resources/Images/Chips.png", pos, Collectible::CollectibleType::CHIPS);
			mEntityManager->addEntity(chips);
			cm->addCollidable(chips);
			Debug::log("Spawning chips at: [" + std::to_string(x) + ", " + std::to_string(y) + "]", Debug::INFO);
}
}
}

void Level::increaseMapBounds(sf::IntRect size) {
	mMapBounds.width += size.width;
}

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget, std::string levelFilename) {
	//assert(aTarget != 0);

	mTarget = aTarget;
	mWindow = &aWindow;

	std::string configText = ResourceManager::getInstance().loadJsonFile(levelFilename);
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	//Initialize entites from a JSON doc
	initializeEntities(mWindow, configDoc);
	
	if (configDoc.HasMember("Level")) {
		int level = configDoc["Level"].GetInt();
		if (level == 1) mapfilepath = "Resources/Configs/Levels/Level1Gatherables.png";
		else if (level == 2) mapfilepath = "Resources/Configs/Levels/Level2Gatherables.png";
		else if (level == 3) mapfilepath = "Resources/Configs/Levels/Level3Gatherables.png";
	}

	// Initialize eggs, chips and ludd from a map
	readInitMap(mapfilepath);

	mPointsOfNoReturn.push_back(mWindow->getSize().x / 2 + 1000.f);
	mCurrentPONR = mWindow->getView().getSize().x / 2;
	//SoundEngine::getInstance().playMusic("resources/music/musik16.wav");

}

void Level::tick(const sf::Time& deltaTime) {
	updateView(deltaTime);
	if (mTimeStunned <= 0) {
	/*	mEffectInterval -= deltaTime.asSeconds();
		if (mEffectInterval <= 0) {
			createEffects();
			mEffectInterval = (float)(rand() % 5);
		}*/
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}
}

void Level::updateView(const Time& deltaTime) {
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
	
	if (xPos < screenXPos - X_OFFSET / 2) {
		deltaMove.x *= -1;
	}
	else if (xPos < screenXPos + X_OFFSET / 2) {
		deltaMove.x = 0;
	}

	if (yPos < screenYPos - Y_OFFSET / 2) {
		deltaMove.y *= -1;
	}
	else if (yPos < screenYPos + Y_OFFSET / 2) {
		deltaMove.y = 0;
	}

	if (screenXPos >= minX && screenXPos <= maxX) {
		if (!mPointsOfNoReturn.empty() && screenXPos >= mPointsOfNoReturn.back()) {
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

#ifdef _DESIGNER_HAX_
	view.setCenter(mTarget->getPosition());
#endif // _DESIGNER_HAX_

	int progress = (int)((mTarget->getPosition().x / mMapBounds.width)*7);
	if (mProgress != progress) {
		mProgress = progress;
		//TODO: make dynamic (add current level event to setup file)
		static const char* parameter = "Progress";
		SoundEngine::getInstance().setEventParameter("event:/MUSIK/Bana_1", parameter, (float)mProgress);
	}
	mWindow->setView(view);
}
bool Level::isAlive() const {
	return true;
}

bool Level::isActive() const {
	return mIsActive;
}

void Level::setActive(const bool& active) {
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
		float xMin = (mMapBounds.width / max) * (i + 1) - mWindow->getView().getSize().x * 3;
		float xMax = (mMapBounds.width / max) * (i + 1) + mWindow->getView().getSize().x * 3;

		if (x < xMax && x >= xMin) {
			target.draw(mBackgroundImages[i]);
		}
	}
}

//TODO: move to an effect creator, as level has
//more than enough responsibilities.
/*void Level::createEffects() {
	//Background effect
	sf::Vector2f vec = VectorMath::normalizeVector(sf::Vector2f(-0.5f, 1.0f));
	for (int i = 0; i <= rand() % 20; i++) {
		BackgroundEffect* eff = new BackgroundEffect(EFFECT_FILEPATH, vec*EFFECT_SPEED, sf::Vector2f((float)(rand() % 15000), -100) + vec*EFFECT_SPEED / 3.0f, EFFECT_LIFETIME, mTarget);
		mEntityManager->addEntity(eff);
	}
}*/

void Level::stun(const sf::Time& deltatime) {
	mTimeStunned = deltatime.asSeconds();
}