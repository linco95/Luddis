#include "Luddis.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "SoundEngine.h"
#include "VectorMath.h"
#include "Projectile.h"
#include <SFML/System.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <array>

static const char* ANIMATION_FILEPATH = "resources/images/spritesheets/Grafik_Luddis_walkcykle_sprite_longer_version_s2d2v3.png";
static const char* ANIMATION_HIT = "resources/images/spritesheets/Grafik_Luddis_hit_sprite_s2d2v1.png";
static const std::string SOUND_FILENAME1 = "Resources/Audio/Skott_Blås_Små_01.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/Skott_Blås_Små_02.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/Skott_Blås_Små_03.wav";

static const std::string MUSIC_FILENAME1 = "Resources/Music/Mists_of_Time-4T.ogg";
static const std::string MUSIC_FILENAME2 = "Resources/Music/The_Abyss-4T.ogg";

//This should be dynamic later to determine what texture to use for projectiles
static const std::array<std::string, 3> PROJECTILE_FILENAME = { "Resources/Images/Grafik_Attack 1_35x35_s1d3v1.png",
												   "Resources/Images/Grafik_Attack 2_35x35_s1d3v1.png",
												   "Resources/Images/Grafik_Attack 3_35x35_s1d3v1.png"
												 };

//All float times are in seconds
static const float PROJECTILE_RELOAD = 0.1f;
static const float PROJECTILE_TIMER = 3.0f;
static const float GRACEAREA = 12;
static const float MOVESPEED = 200;
static const float PROJECTILE_SPEED = 300;
static const float MUZZLEOFFSET = 50.0f;
static const sf::Vector2f FRONTVECTOR(1, 0);
static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static int LIFE = 10;

Luddis::Luddis(std::string textureFilename, sf::RenderWindow* window) : 
	mIsAlive(true), 
	mWindow(window), 
	mProjectileCooldown(0), 
	mStunDuration(1),
	// Magic constants below are just temporary, until the file manager is created and implemented with the animation
	mAnimation(Animation(ANIMATION_FILEPATH, sf::Vector2i(104, 90), 16, 16, sf::seconds(0.1f))),
	mColliding(false),
	mPrevPos(0, 0)
{
	setPosition(mWindow->getView().getSize().x / 2, mWindow->getView().getSize().y / 2);
}

Luddis::~Luddis(){

}
bool Luddis::isAlive() {
	return mIsAlive;
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
	mAnimation.tick(deltaTime);
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
	sf::Vector2f offset(VectorMath::normalizeVector(direction));
	float moveX(offset.x*deltaTime.asSeconds()*MOVESPEED);
	float moveY(offset.y*deltaTime.asSeconds()*MOVESPEED);
	
	sf::Vector2f tempPos = this->getPosition();

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
	}
	//Handle keyboard presses
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		SoundEngine::getInstance().fadeToNewMusic(MUSIC_FILENAME1, 2.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		SoundEngine::getInstance().fadeToNewMusic(MUSIC_FILENAME2, 2.0f);
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

void Luddis::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == BG_SOLID){
		mColliding = true;
		mCollideBox = collidable->getHitBox();
	}
	if (collidable->getCollisionCategory() == BG_DAMAGE){
		LIFE -= 1;
	}
	if (collidable->getCollisionCategory() == ENEMY) {
		mAnimation.addAnimation(Animation(ANIMATION_HIT, sf::Vector2i(120, 90), 4, 4, sf::seconds(0.1f)));
		LIFE -= 1;
	}
	if (collidable->getCollisionCategory() == COLLECT){
		
	}
}

sf::FloatRect Luddis::getHitBox(){
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}

Entity::RenderLayer Luddis::getRenderLayer() const {
	return LAYER;
}
