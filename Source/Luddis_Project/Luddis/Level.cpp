#include "Level.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "Silverfish.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "Utils.h"
#include <SFML\Audio.hpp>
#include <rapidjson\document.h>
#include <array>
using namespace sf;

static const float X_OFFSET = 200.f,
				   Y_OFFSET = 50.f,
				   SCROLLSPEED = 120.f;

static const Entity::RenderLayer LAYER = Entity::RenderLayer::BACKGROUND;
static std::string BGFILEPATH = "resources/images/Grafik_Bana1,1Final_S2D3V1.png";
static const std::array<std::string, 3> CONFIGMEMBERS = { "Background_file", "Silverfish_spawns", "Boss_config" };
Level::Level():
mIsActive(true)
{

}

Level::~Level(){

}
#include <iostream>
void Level::initializeEntities(){
	EntityManager* em = &EntityManager::getInstance();
	CollisionManager* cm = &CollisionManager::getInstance();

	std::string configText = LuddisUtilFuncs::loadJsonFile("resources/configs/levels/level01.json");
	rapidjson::Document configDoc;
	configDoc.Parse(configText.c_str());
	assert(configDoc.IsObject());
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

		Silverfish* fish = new Silverfish(mWindow, sf::Vector2f((float)(*itr)["x"].GetDouble(), (float)(*itr)["y"].GetDouble()));
		em->addEntity(fish);
		cm->addCollidable(fish);
		std::cout << (*itr)["x"].GetDouble() << " " << (*itr)["y"].GetDouble() << std::endl;
	}
	assert(configDoc["Background_file"].IsString());
	std::cout << configDoc["Background_file"].GetString();
	BGFILEPATH = configDoc["Background_file"].GetString();



}
void Level::initializeLevel(sf::RenderWindow& aWindow, Transformable* aTarget){
	//assert(aTarget != 0);

	mTarget = aTarget;
	mWindow = &aWindow;

	initializeEntities();

	ResourceManager::getInstance().loadTexture(BGFILEPATH, IntRect(Vector2<int>(), Vector2<int>(Texture::getMaximumSize(), Texture::getMaximumSize())));
	mBackground.setTexture(ResourceManager::getInstance().getTexture(BGFILEPATH));

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
	float maxX = mBackground.getTextureRect().width - view.getSize().x / 2;
	float minY = view.getSize().y / 2;
	float maxY = mBackground.getTextureRect().height - view.getSize().y / 2;
	
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
	target.draw(mBackground);
}
