#include "Luddis.h"
#include "Inventory.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "GameStateLevel.h"
#include "GameManager.h"
#include "GUIManager.h"
#include "CollisionManager.h"
#include "SoundEngine.h"
#include "VectorMath.h"
#include "LuddisStatePlayable.h"
#include "Projectile.h"
#include "Dialogue.h"
#include "ViewUtility.h"
#include <SFML/System.hpp>
#include <cmath>
#include <string>
#include <array>

static const std::string ANIMATION_FILEPATH = "Resources/Images/Spritesheets/Luddis_walkcykle";

static const sf::Vector2f FRONTVECTOR(1, 0);

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(35, 8);

//TEMP
static Animation STASISANIMATION("Resources/Images/Spritesheets/Stasis_ring");


Luddis::Luddis(std::string textureFilename, sf::RenderWindow* window, EntityManager* entityManager) :
	mIsAlive(true),
	mIsActive(true),
	mWindow(window),
	mEntityManager(entityManager),
	mAnimation(ANIMATION_FILEPATH),
	mPrevPos(0, 0),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
	mLife(2)
{
	setPosition(mWindow->getView().getSize().x / 2, mWindow->getView().getSize().y / 2);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	//sf::Mouse::setPosition(mWindow->mapCoordsToPixel(getPosition()));
	
	Inventory::getInstance().addDust(mLife);
	mCurrentLuddState = new LuddisStatePlayable(this, mWindow, mEntityManager);
}

Luddis::~Luddis(){
	delete mHitbox;
}

bool Luddis::isAlive() const{
	return mIsAlive;
}

bool Luddis::isActive() const{
	return mIsActive;
}

void Luddis::setActive(const bool& active){
	mIsActive = active;
}

void Luddis::tick(const sf::Time& deltaTime){
	//Update the currently active state, unless null pointer.
	if(mCurrentLuddState!= nullptr)
		mCurrentLuddState->tick(deltaTime);
	
	
	mAnimation.tick(deltaTime);

	// Temporary
	static const float ROTATIONSPEED = 75.f;
	static int x = 0;
	static const float interval = 1.f;
	static const float pulseSpeed = 0.5f;

	STASISANIMATION.setScale(getScale() + sf::Vector2f(1,1) * interval * ((float)std::sin(x * pulseSpeed) + 0.5f) * deltaTime.asSeconds());
	STASISANIMATION.setPosition(getPosition());
	STASISANIMATION.rotate(ROTATIONSPEED * deltaTime.asSeconds());
	STASISANIMATION.tick(deltaTime);
	x++;
	/*(x >= 10) ? x = 0 : x++;*/
	// Update scale

}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(STASISANIMATION, states);
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);

}

Luddis::Category Luddis::getCollisionCategory(){
	return PLAYER_OBJECT;
}

Luddis::Type Luddis::getCollisionType(){
	return REC;
}

void Luddis::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (mCurrentLuddState != nullptr)
		mCurrentLuddState->collide(collidable, moveAway);

}

// Change luddis' size depending on the amount of dust that the inventory has


void Luddis::setPlayerState(LuddisState * luddisState){
	delete mCurrentLuddState;
	mCurrentLuddState = luddisState;
}

sf::FloatRect Luddis::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}

sf::Shape* Luddis::getNarrowHitbox() const{
	mHitbox->setPosition(getPosition());
	mHitbox->setRotation(getRotation());
	mHitbox->setScale(getScale());
	return mHitbox;
}
Entity::RenderLayer Luddis::getRenderLayer() const {
	return LAYER;
}

void Luddis::reset(GameStateLevel* gameStateLevel) {
	//TODO: Make this create a gameover menu 
	mGameStateLevel->resetLevel();
	GameManager::getInstance().setGameState(mGameStateLevel);
}

AnimationQueue* Luddis::getAnimation(){
	return &mAnimation;
}
