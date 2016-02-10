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
#include "EntityManager.h"
#include "CollisionManager.h"
#include "Utils.h"
#include <SFML\Audio.hpp>
#include <array>
using namespace sf;

static const float X_OFFSET = 200.f,
				   Y_OFFSET = 50.f,
				   SCROLLSPEED = 200;

static const std::array<std::string, 8> CONFIGMEMBERS = { "Background", "Silverfish_spawns", "Obstacle_spawns", "Chips_spawns", "Dust_spawns", "Boss_spawns", "Spider_spawns", "Boss_config" };
static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
Level::Level() :
mIsActive(true)
{

}
Level::~Level(){

}

#include <iostream>
void Level::initializeEntities(sf::RenderWindow* window, const rapidjson::Document& configDoc){
	EntityManager* em = &EntityManager::getInstance();
	CollisionManager* cm = &CollisionManager::getInstance();
	ResourceManager* rm = &ResourceManager::getInstance();

	for (std::size_t i = 0; i < CONFIGMEMBERS.size(); i++){
		assert(configDoc.HasMember(CONFIGMEMBERS[i].c_str()));
	}
	const rapidjson::Value& fishSpawns = configDoc["Silverfish_spawns"];
	assert(fishSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = fishSpawns.Begin(); itr != fishSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());

		Silverfish* fish = new Silverfish(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["angle"].GetDouble());
		em->addEntity(fish);
		cm->addCollidable(fish);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

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
		em->addEntity(obstacle);
		cm->addCollidable(obstacle);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	const rapidjson::Value& chipsSpawns = configDoc["Chips_spawns"];
	assert(chipsSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = chipsSpawns.Begin(); itr != chipsSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());

		Chips* chips = new Chips(mWindow, (*itr)["filename"].GetString(), sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["angle"].GetDouble());
		em->addEntity(chips);
		cm->addCollidable(chips);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	const rapidjson::Value& dustSpawns = configDoc["Dust_spawns"];
	assert(dustSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = dustSpawns.Begin(); itr != dustSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("filename") && (*itr)["filename"].IsString());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());
		assert(itr->HasMember("angle") && (*itr)["angle"].IsDouble());

		Dust* dust = new Dust(mWindow, (*itr)["filename"].GetString(), sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()), (float)(*itr)["angle"].GetDouble());
		em->addEntity(dust);
		cm->addCollidable(dust);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	const rapidjson::Value& bossSpawns = configDoc["Boss_spawns"];
	assert(bossSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = bossSpawns.Begin(); itr != bossSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());

		BossDishCloth* boss = new BossDishCloth(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()));
		em->addEntity(boss);
		cm->addCollidable(boss);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

	const rapidjson::Value& spiderSpawns = configDoc["Spider_spawns"];
	assert(spiderSpawns.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = spiderSpawns.Begin(); itr != spiderSpawns.End(); itr++){
		assert(itr->IsObject());
		assert(itr->HasMember("x") && (*itr)["x"].IsInt());
		assert(itr->HasMember("y") && (*itr)["y"].IsInt());

		Spider* spider = new Spider(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()));
		em->addEntity(spider);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}

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

void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	mWindow = &aWindow;

	std::string configText = LuddisUtilFuncs::loadJsonFile("resources/configs/levels/level01.json");
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());

	initializeEntities(mWindow, configDoc);

	mPointsOfNoReturn.push_back(mWindow->getSize().x / 2 + 1000.f);
	mCurrentPONR = mWindow->getView().getSize().x / 2;
	//SoundEngine::getInstance().playMusic("resources/music/musik16.wav");

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
