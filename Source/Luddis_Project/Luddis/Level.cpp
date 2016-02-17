#include "Level.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "Silverfish.h"
#include "BossDishCloth.h"
#include "Luddis.h"
#include "Spider.h"
#include "Chips.h"
#include "Dust.h"
#include "Obstacle.h"
#include "SpiderEgg.h"
#include "ViewUtility.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include <SFML\Audio.hpp>
#include <array>
#include <map>
#include <iostream>

using namespace sf;

static const float X_OFFSET = 200.f,
				   Y_OFFSET = 50.f,
				   SCROLLSPEED = 200;

static const std::array<std::string, 6> CONFIGMEMBERS = { "Background", "Silverfish_spawns", "Obstacle_spawns", "Boss_spawns", "Spider_spawns", "Boss_config" };
static const char* mapfilepath = "Resources/Configs/Levels/Level01MAP.png";
static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
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
} ;
spawnableInfo spawns[] {spawnableInfo(sf::Color(0, 0, 0), CHIPS), spawnableInfo(sf::Color(255, 0, 0), DUST), spawnableInfo(sf::Color(0, 0, 255), EGG)};
Level::Level(EntityManager* entityManager) :
mIsActive(true),
mEntityManager(entityManager)
{

}
Level::~Level(){
	
}

#include "Debug.h"
void Level::readInitMap(){
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
		std::cout << CONFIGMEMBERS[i] << std::endl;
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}
	// Silverfishes
	const rapidjson::Value& fishSpawns = configDoc["Silverfish_spawns"];
	assert(fishSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = fishSpawns.Begin(); itr != fishSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());
		assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());

		Silverfish* fish = new Silverfish(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["angle"].GetDouble(), (float)(*itr)["activationpos"].GetDouble(), mTarget);
		mEntityManager->addEntity(fish);
		cm->addCollidable(fish);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	// Obstacles
	const rapidjson::Value& obstacleSpawns = configDoc["Obstacle_spawns"];
	assert(obstacleSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = obstacleSpawns.Begin(); itr != obstacleSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
		assert(itr->HasMember("type") && (*itr)["type"].IsInt());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());

		Obstacle* obstacle = new Obstacle(mWindow, (*itr)["filename"].GetString(), Obstacle::ObstacleType((*itr)["type"].GetInt()), sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["angle"].GetDouble());
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

		BossDishCloth* boss = new BossDishCloth(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["activationpos"].GetDouble(), mTarget, mEntityManager);
		mEntityManager->addEntity(boss);
		cm->addCollidable(boss);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	// The spider
	const rapidjson::Value& spiderSpawns = configDoc["Spider_spawns"];
	assert(spiderSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = spiderSpawns.Begin(); itr != spiderSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("activationpos") && (*itr)["activationpos"].IsInt());

		Spider* spider = new Spider(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["activationpos"].GetDouble(), mTarget);
		mEntityManager->addEntity(spider);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
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
		std::cout << "Filepath: "<< BGFILEPATH <<"\n";
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
