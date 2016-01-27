#include "Luddis.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "VectorMath.h"
#include "Projectile.h"
#include <SFML/System.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

static const std::string SOUND_FILENAME1 = "Resources/Audio/MGF-99-MULTI-Doepfer Modular - Classic Acid C2.wav";

//This should be dynamic later to determine what texture to use for projectiles
static const std::string PROJECTILE_FILENAME = "Resources/Images/Projectile.png";

//All float values are in milliseconds
static const float PROJECTILE_RELOAD = 1000;
static const float PROJECTILE_TIMER = 3000;
static const float graceArea = 12;

Luddis::Luddis(std::string textureFilename, sf::Window* window) : 
	mIsAlive(true), 
	mWindow(window), 
	mProjectileCooldown(0), 
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mTestSound1(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME1))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	mSprite.setPosition((float)mWindow->getSize().x / 2, (float)mWindow->getSize().y / 2);
}

Luddis::~Luddis(){

}
bool Luddis::isAlive() const{
	return mIsAlive;
}
void Luddis::tick(const sf::Time& deltaTime){
	mProjectileCooldown -= deltaTime.asMilliseconds();
	handleInput(deltaTime);
	updateRotation();
}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

sf::Vector2f Luddis::getVectorMouseToSprite() const{
	sf::Vector2f playerPosition(mSprite.getPosition());
	sf::Vector2f mousePosition(sf::Mouse::getPosition(*mWindow));
	return mousePosition - playerPosition;
}

void Luddis::updateMovement(const sf::Time& deltaTime){
	sf::Vector2f direction = getVectorMouseToSprite();
	//Only move if not close to the cursor position
	if (VectorMath::getVectorLengthSq(direction) > graceArea){
		sf::Vector2f offset(VectorMath::normalizeVector(direction));
		mSprite.move(offset.x*deltaTime.asSeconds(), offset.y*deltaTime.asSeconds());
	}
}

#include <iostream>

void Luddis::updateRotation(){
	sf::Vector2f direction = getVectorMouseToSprite();
	float rotation = std::atan2f(direction.x, -direction.y) * 180 / (float)M_PI;
	mSprite.setRotation(rotation);
}

void Luddis::attack(){
	sf::Vector2f direction = VectorMath::normalizeVector( getVectorMouseToSprite());
	sf::Vector2f muzzlePoint = mSprite.getPosition() + (direction * (float)10);
	EntityManager::getInstance().addEntity(new Projectile(PROJECTILE_FILENAME,
		direction, muzzlePoint, PROJECTILE_TIMER));
	mProjectileCooldown = PROJECTILE_RELOAD;
}

void Luddis::handleInput(const sf::Time& deltaTime){
	//Handle mouse clicks
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true){
		updateMovement(deltaTime);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) == true
		&& mProjectileCooldown<=0){
		attack();
	}
	//Handle keyboard clicks
	static bool isPlaying = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !isPlaying){
		std::cout << "Playing" << std::endl;
		mTestSound1.play();
		isPlaying = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		
	}



	/*if (mTestSound1.getStatus() == sf::Sound::Playing){
		mTestSound1.stop();
	}*/
}