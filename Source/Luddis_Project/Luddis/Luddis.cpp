#include "Luddis.h"
#include "Inventory.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "SoundEngine.h"
#include "VectorMath.h"
#include "Projectile.h"
#include "Dialogue.h"
#include <SFML/System.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <array>

static const char* ANIMATION_FILEPATH = "resources/images/spritesheets/Grafik_Luddis_walkcykle_sprite_longer_version_s2d2v3.png";
static const Animation HIT_ANIMATION = Animation("resources/images/spritesheets/Grafik_Luddis_hit_sprite_s2d2v1");
static const Animation SHOT_ANIMATION = Animation("resources/images/spritesheets/Grafik_Luddis shot120x90treframes_s2d3v1");
static const std::string SOUND_FILENAME1 = "Resources/Audio/Skott_Bl�s_Sm�_01.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/Skott_Bl�s_Sm�_02.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/Skott_Bl�s_Sm�_03.wav";

static const std::string POWER_DISPLAY = "Resources/Images/PowerDisplay.png";
static const std::string BUTTON_TEXTURE = "Resources/Images/Button.png";

//This should be dynamic later to determine what texture to use for projectiles
static const std::array<std::string, 3> PROJECTILE_FILENAME = { "Resources/Images/Grafik_Attack 1_35x35_s1d3v1.png",
												   "Resources/Images/Grafik_Attack 2_35x35_s1d3v1.png",
												   "Resources/Images/Grafik_Attack 3_35x35_s1d3v1.png"
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

Luddis::Luddis(std::string textureFilename, sf::RenderWindow* window) : 
	mIsAlive(true), 
	mIsActive(true),
	mWindow(window), 
	mProjectileCooldown(0), 
	mStunDuration(0),
	mLoseDust(1),
	// Magic constants below are just temporary, until the file manager is created and implemented with the animation
	mAnimation(Animation(ANIMATION_FILEPATH, sf::Vector2i(104, 90), 16, 16, sf::seconds(0.1f))),
	mColliding(false),
	mPrevPos(0, 0),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	setPosition(mWindow->getView().getSize().x / 2, mWindow->getView().getSize().y / 2);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	sf::Mouse::setPosition(sf::Vector2i((int)getPosition().x, (int)getPosition().y));
	mPowerups[0] = new PowerupDisplay(POWER_DISPLAY, sf::Vector2f((float)mWindow->getSize().x * 2 / 5, (float)mWindow->getSize().y - 80), 15.0f);
	
	//Adds a display of the first power that luddis has
	EntityManager::getInstance().addEntity(mPowerups[0]);
	Dialogue* dialogue = new Dialogue("This is a dialogue. If the text is too long it will wrap around! It can also have buttons attached to it.", mWindow, sf::Vector2f(100, 100));
	EntityManager::getInstance().addEntity(dialogue);
	dialogue->addButton(BUTTON_TEXTURE, sf::Vector2f(200, 300));
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
}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

sf::Vector2f Luddis::getVectorMouseToSprite() const{
	sf::Vector2f playerPosition(getPosition());
	sf::Vector2i terst = sf::Mouse::getPosition(*mWindow);
	sf::Vector2f mousePosition(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
	return mousePosition - playerPosition;
}

void Luddis::updateMovement(const sf::Time& deltaTime){
	sf::Vector2f direction = getVectorMouseToSprite();
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
	float rotation = VectorMath::getAngle(sf::Vector2f(FRONTVECTOR), direction) * 180 / (float)M_PI;
	setRotation(rotation);

	static bool isFlipped = false;
	if (direction.x <= 0 && !isFlipped){
		scale(sf::Vector2f(1, -1));
		isFlipped = true;
	}
	else if (direction.x > 0 && isFlipped){
		scale(sf::Vector2f(1, -1));
		isFlipped = false;
	}
}

void Luddis::attack(){
	sf::Vector2f direction = VectorMath::normalizeVector(getVectorMouseToSprite()) * PROJECTILE_SPEED;
	sf::Vector2f muzzlePoint = getPosition() + direction * MUZZLEOFFSET / PROJECTILE_SPEED;
	mProjectileCooldown = PROJECTILE_RELOAD;
	int randValue = std::rand() % PROJECTILE_FILENAME.max_size();
	Projectile *proj = new Projectile(PROJECTILE_FILENAME[randValue], direction, muzzlePoint, PROJECTILE_TIMER, HAIR);
	
	EntityManager::getInstance().addEntity(proj);
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
		if (mPowerups[0]->getCooldown() <= 0){
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

void Luddis::collide(CollidableEntity *collidable){
	if (collidable->getCollisionCategory() == BG_SOLID){
		mColliding = true;
		mCollideBox = collidable->getHitBox();
	}
	if (collidable->getCollisionCategory() == BG_DAMAGE){
		mAnimation.replaceAnimation(HIT_ANIMATION);
		if (mLoseDust < 0){
			Inventory::getInstance().removeDust(1);
			mLoseDust = 1.0f;
		}
	}
	if (collidable->getCollisionCategory() == ENEMY) {
		mAnimation.replaceAnimation(HIT_ANIMATION);
		if (mLoseDust < 0){
			Inventory::getInstance().removeDust(1);
			mLoseDust = 1.0f;
		}
	}
	if (collidable->getCollisionCategory() == COLLECT){
		
	}
	if (collidable->getCollisionCategory() == ENEMY_STUN){
		if (mStunDuration <= 0){
			mStunDuration = 1.0f;
			mAnimation.replaceAnimation(HIT_ANIMATION);
		}
	}
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
