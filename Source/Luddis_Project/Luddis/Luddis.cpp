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
static const std::string PROJECTILE_FILENAME = "Resources/Images/Turd.png";

static const sf::Time PROJECTILE_TIMER = sf::seconds(3);
static const float graceArea = 12;

Luddis::Luddis(std::string textureFilename, sf::Window* window) : 
	mIsAlive(true), 
	mWindow(window), 
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mTestSound1(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME1))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
}

Luddis::~Luddis(){

}
bool Luddis::isAlive() const{
	return mIsAlive;
}
void Luddis::tick(const sf::Time& deltaTime){
	handleInput();
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

void Luddis::updateMovement(){
	sf::Vector2f direction = getVectorMouseToSprite();
	//Only move if not close to the cursor position
	if (VectorMath::getVectorLengthSq(direction) > graceArea){
		sf::Vector2f offset(VectorMath::normalizeVector(direction));
		mSprite.move(offset.x, offset.y);
	}
}

void Luddis::updateRotation(){
	sf::Vector2f direction = getVectorMouseToSprite();
	float rotation = std::atan2f(direction.x, -direction.y) * 180 / (float)M_PI;
	mSprite.setRotation(rotation);
}

void Luddis::attack(){
	sf::Vector2f direction = VectorMath::normalizeVector( getVectorMouseToSprite());

	EntityManager::getInstance().addEntity(new Projectile(PROJECTILE_FILENAME,
		direction, PROJECTILE_TIMER));
}

void Luddis::handleInput(){
	//Handle mouse clicks
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true){
		updateMovement();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) == true){
		attack();
	}
	//Handle keyboard clicks
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		mTestSound1.play();
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