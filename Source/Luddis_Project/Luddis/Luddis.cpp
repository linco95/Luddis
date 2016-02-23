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
#include "Projectile.h"
#include "Dialogue.h"
#include "ViewUtility.h"
#include <SFML/System.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <array>

static const std::string ANIMATION_FILEPATH = "Resources/Images/Spritesheets/Luddis_walkcykle";
static const std::string ANIMATION_ALMOSTDEAD = "Resources/Images/Spritesheets/luddis_CriticalHealth";
//static const Animation ANIMATION_FILEPATH = Animation("resources/images/spritesheets/Sprite_Debug_480x205");
static const std::string HIT_ANIMATION = "Resources/Images/Spritesheets/Luddis_hit";
static const std::string SHOT_ANIMATION = "Resources/Images/Spritesheets/Luddis_shot";
static const std::string SOUND_FILENAME1 = "Resources/Audio/Skott_Blås_Små_01.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/Skott_Blås_Små_02.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/Skott_Blås_Små_03.wav";
//This should be dynamic later to determine what texture to use for projectiles
static const std::array<std::string, 3> PROJECTILE_FILENAME = { "Resources/Images/Luddis_attack1.png",
												   "Resources/Images/Luddis_attack2.png",
												   "Resources/Images/Luddis_attack3.png"
												 };

//All float times are in seconds
static const float PROJECTILE_RELOAD = 0.4f;
static const float PROJECTILE_TIMER = 3.0f;
static const float GRACEAREA = 12;
static const float MOVESPEED = 200;
static const float PROJECTILE_SPEED = 300;
static const float MUZZLEOFFSET = 50.0f;
static const sf::Vector2f FRONTVECTOR(1, 0);
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(35, 8);


Luddis::Luddis(std::string textureFilename, sf::RenderWindow* window, EntityManager* entityManager) :
	mIsAlive(true), 
	mIsActive(true),
	mWindow(window), 
	mProjectileCooldown(0), 
	mStunDuration(0),
	mLoseDust(1),
	mEntityManager(entityManager),
	mAnimation(ANIMATION_FILEPATH),
	mColliding(false),
	mPrevPos(0, 0),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
	mScale(1,1),
	mIsFlipped(false),
	mLife(2)
{
	setPosition(mWindow->getView().getSize().x / 2, mWindow->getView().getSize().y / 2);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	//sf::Mouse::setPosition(mWindow->mapCoordsToPixel(getPosition()));
	
	Inventory::getInstance().addDust(mLife);

	//Adds a display of the first power that luddis has. Some different class might want to handle this
	
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

	if (mProjectileCooldown >= 0){
		mProjectileCooldown -= deltaTime.asSeconds();
	}
	if (mStunDuration >= 0){
		mStunDuration -= deltaTime.asSeconds();
	}
	else if(mStunDuration<0){
		handleInput(deltaTime);
		updateRotation();
	}
	if (mLoseDust >= 0){
		mLoseDust -= deltaTime.asSeconds();
	}
	mAnimation.tick(deltaTime);
	// Update scale
	changeScale();

}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

sf::Vector2f Luddis::getVectorMouseToSprite() const{
	sf::Vector2f playerPosition(getPosition());
	sf::Vector2f mousePosition(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
	return mousePosition - playerPosition;
}

void Luddis::updateMovement(const sf::Time& deltaTime){
	sf::Vector2f direction = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(direction) == 0) return;
	sf::Vector2f offset(VectorMath::normalizeVector(direction));
	float moveX(offset.x*deltaTime.asSeconds()*MOVESPEED);
	float moveY(offset.y*deltaTime.asSeconds()*MOVESPEED);
	
	sf::Vector2f tempPos = getPosition();

	// Not colliding
	if (mColliding == false){
		//Only move if not close to the cursor position
		if (VectorMath::getVectorLengthSq(direction) > GRACEAREA){
			move(moveX, moveY);
		}
	}

	// Colliding
	else if (mColliding == true){
		// mCollideBox - variable to handle the object luddis collides with
		
		// Get sides of obstacle as vectors
		//Top left (1)
		sf::Vector2f tl(mCollideBox.top, mCollideBox.left);
		//Top right (2)
		sf::Vector2f tr(mCollideBox.top, mCollideBox.left + mCollideBox.width);

		//Normalize vectors
		sf::Vector2f temp1 = VectorMath::normalizeVector(tl);
		sf::Vector2f temp2 = VectorMath::normalizeVector(tr);

		// Check which side luddis is crashing into and move along the other one
		if (!mCollideBox.contains(mPrevPos + temp1)){
			// Use temp2
			moveX = (temp2.x*deltaTime.asSeconds()*MOVESPEED);
			moveY = (temp2.y*deltaTime.asSeconds()*MOVESPEED);
			}
		else if (!mCollideBox.contains(mPrevPos + temp2)){
			// Use temp1
			moveX = (temp1.x*deltaTime.asSeconds()*MOVESPEED);
			moveY = (temp1.y*deltaTime.asSeconds()*MOVESPEED);
		}
		move(moveX, moveY);
	}
	mColliding = false;

	mPrevPos = tempPos;
}

#include <iostream>

void Luddis::updateRotation(){
	
	sf::Vector2f direction = getVectorMouseToSprite();
	//float rotation = std::atan2f(direction.x, -direction.y) * 180 / (float)M_PI;
	float rotation = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
	setRotation(rotation);

	if (direction.x <= 0 && !mIsFlipped){
		scale(sf::Vector2f(1, -1));
		mIsFlipped = true;
	}
	else if (direction.x > 0 && mIsFlipped){
		scale(sf::Vector2f(1, -1));
		mIsFlipped = false;
	}
}

void Luddis::attack(){
	sf::Vector2f direction = VectorMath::rotateVector(FRONTVECTOR, getRotation());
	sf::Vector2f muzzlePoint = getPosition() + direction * MUZZLEOFFSET;
	mProjectileCooldown += PROJECTILE_RELOAD;
	int randValue = std::rand() % PROJECTILE_FILENAME.max_size();
	Projectile *proj = new Projectile(PROJECTILE_FILENAME[randValue], direction  * PROJECTILE_SPEED, muzzlePoint, PROJECTILE_TIMER, HAIR);
	
	mEntityManager->addEntity(proj);
	CollisionManager::getInstance().addCollidable(proj);
	// Pull out constant variable
	SoundEngine::getInstance().playSound("resources/audio/Luddis_skott_16bit.wav");

}

void Luddis::handleInput(const sf::Time& deltaTime){
	//Handle mouse clicks
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true){
		updateMovement(deltaTime);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) == true
		&& mProjectileCooldown <= 0){
		attack();
		mAnimation.replaceAnimation(SHOT_ANIMATION);
	}
	//Handle keyboard presses
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		if (mPowerups[0] != 0 && mPowerups[0]->getCooldown() <= 0){
			mPowerups[0]->activateCooldown();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){

	}
}

Luddis::Category Luddis::getCollisionCategory(){
	return FRIEND;
}

Luddis::Type Luddis::getCollisionType(){
	return REC;
}

void Luddis::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	// Collision with solid object
	if (collidable->getCollisionCategory() == BG_SOLID){
		mColliding = true;
		mCollideBox = collidable->getHitBox();
	}
	// Collision with damaging object
	if (collidable->getCollisionCategory() == BG_DAMAGE){
		mAnimation.replaceAnimation(HIT_ANIMATION);
		if (mLoseDust < 0) {
			Inventory::getInstance().addDust(-1);
			mLoseDust = 1.0f;
			if (Inventory::getInstance().getDust() <= 0){
				mIsAlive = false;
			}
		}
	}
	// Collision with an enemy
	if (collidable->getCollisionCategory() == ENEMY) {
		mAnimation.replaceAnimation(HIT_ANIMATION);
		if (mLoseDust < 0){
			Inventory::getInstance().addDust(-1);
			mLoseDust = 1.0f;
			if (Inventory::getInstance().getDust() <= 0) {
				mIsAlive = false;
			}
		}
	}
	// Collision with an collectible
	if (collidable->getCollisionCategory() == COLLECT){
		
	}
	// Collision with a stunning entity
	if (collidable->getCollisionCategory() == ENEMY_STUN){
		if (mStunDuration <= 0){
			mStunDuration = 1.0f;
			mAnimation.replaceAnimation(HIT_ANIMATION);
		}
	}
}

// Change luddis' size depending on the amount of dust that the inventory has
void Luddis::changeScale(){
	int dust = Inventory::getInstance().getDust();
	if (dust < 2 && mScale != sf::Vector2f(1.f, 1.f)){
		mScale.x = 1.0f;
		mScale.y = 1.0f;
		mAnimation.setDefaultAnimation(ANIMATION_ALMOSTDEAD);
	}
	else if (dust < 4 && dust > 1 && mScale != sf::Vector2f(1.25f, 1.25f)){
		mScale.x = 1.25f;
		mScale.y = 1.25f;
		mAnimation.setDefaultAnimation(ANIMATION_FILEPATH);
	}
	else if (dust < 6 && dust > 3 && mScale != sf::Vector2f(1.5f, 1.5f)){
		mScale.x = 1.5f;
		mScale.y = 1.5f;
	}
	else if (dust < 8 && dust > 5 && mScale != sf::Vector2f(175.f, 1.75f)){
		mScale.x = 1.75f;
		mScale.y = 1.75f;
	}
	else if (dust > 7 && mScale != sf::Vector2f(2.f, 2.f)){
		mScale.x = 2.0f;
		mScale.y = 2.0f;
	}
	if (mIsFlipped == false)
		setScale(mScale.y, mScale.y);
	if (mIsFlipped == true)
		setScale(mScale.y, -mScale.y);
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
	mGameStateLevel->resetLevel();
	GameManager::getInstance().setGameState(mGameStateLevel);
}