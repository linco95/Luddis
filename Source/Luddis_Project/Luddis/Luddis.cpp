#include "Luddis.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include "Projectile.h"
#include <SFML/System.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

static const char* ANIMATION_FILEPATH = "resources/images/spritesheets/Grafik_Luddis_sprite_walkcycle_120x90_s1d4v1.png";
static const std::string SOUND_FILENAME1 = "Resources/Audio/Skott_Blås_Små_01.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/Skott_Blås_Små_02.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/Skott_Blås_Små_03.wav";

//This should be dynamic later to determine what texture to use for projectiles
static const std::string PROJECTILE_FILENAME1 = "Resources/Images/Grafik_Attack 1_35x35_s1d3v1.png";
static const std::string PROJECTILE_FILENAME2 = "Resources/Images/Grafik_Attack 2_35x35_s1d3v1.png";
static const std::string PROJECTILE_FILENAME3 = "Resources/Images/Grafik_Attack 3_35x35_s1d3v1.png";

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
	// Magic constants below are just temporary, until the file manager is created and implemented with the animation
	mAnimation(ANIMATION_FILEPATH, sf::Vector2i(120, 90), 10, 10, sf::seconds(0.1f)),
	mShotSound1(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME1)),
	mShotSound2(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME2)),
	mShotSound3(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME3)),
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
	mProjectileCooldown -= deltaTime.asSeconds();
	handleInput(deltaTime);
	mAnimation.tick(deltaTime);
	updateRotation();
}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation, states);
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
	
	//sf::Vector2f tempPos = this->getPosition();

	// Not colliding
	if (mColliding == false){
		//Only move if not close to the cursor position
		if (VectorMath::getVectorLengthSq(direction) > GRACEAREA){
			move(moveX, moveY);
		}
	}
	// Colliding
	else if (mColliding == true){
		// mCollideBox
		
		//this->setPosition(mPrevPos);

		// Luddis glider utmed hindret
		sf::Vector2f tempMove(0, 0);
		while (!mCollideBox.contains(this->getPosition() + tempMove) && tempMove.x < moveX && tempMove.y < moveY){
			if (tempMove.x < moveX && !mCollideBox.contains(this->getPosition() + tempMove + sf::Vector2f(1, 0))){
				tempMove.x++;
			}
			if (tempMove.y < moveY && !mCollideBox.contains(this->getPosition() + tempMove + sf::Vector2f(0, 1))){
				tempMove.y++;
			}
		}

		move(tempMove);

		// Move backwards
		//move(-(offset.x*deltaTime.asSeconds()*MOVESPEED), -(offset.y*deltaTime.asSeconds()*MOVESPEED));
	}
	mColliding = false;

	//mPrevPos = tempPos;
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
	int randValue = std::rand() % 3;
	Projectile *proj = new Projectile(PROJECTILE_FILENAME1, direction, muzzlePoint, PROJECTILE_TIMER);
	switch (randValue){
	case 0:
		mShotSound1.play();
		break;
	case 1:
		mShotSound2.play();
		proj->setTexture(PROJECTILE_FILENAME2);
		break;
	case 2:
		mShotSound3.play();
		proj->setTexture(PROJECTILE_FILENAME3);
		break;
	}
	EntityManager::getInstance().addEntity(proj);
	CollisionManager::getInstance().addCollidable(proj);

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
	static bool isPlaying = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		
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


void Luddis::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == BG){
		mColliding = true;
		mCollideBox = collidable->getHitBox();
	}
}

sf::FloatRect Luddis::getHitBox(){
	sf::Sprite temp = mAnimation.getSprite();
	
	sf::FloatRect rekt = temp.getLocalBounds();
	
	sf::Transform trans(getTransform());

	trans.translate(rekt.left * -1, rekt.top * -1);

	rekt = trans.transformRect(rekt);
	
	return rekt;
}

	/*if (mTestSound1.getStatus() == sf::Sound::Playing){
		mTestSound1.stop();
	}*/

Entity::RenderLayer Luddis::getRenderLayer() const {
	return LAYER;
}
