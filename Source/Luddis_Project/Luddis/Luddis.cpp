#include "Luddis.h"
#include "ResourceManager.h"
#include "VectorMath.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

static const std::string SOUND_FILENAME1 = "Resources/Audio/MGF-99-MULTI-Doepfer Modular - Classic Acid C2.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/MGF-99-MULTI-Doepfer Modular - Classic Acid C3.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/MGF-99-MULTI-Doepfer Modular - Classic Acid C4.wav";
static const std::string SOUND_FILENAME4 = "Resources/Audio/MGF-99-MULTI-Doepfer Modular - Classic Acid E1.wav";

static const float graceArea = 12;

Luddis::Luddis(std::string textureFileName, sf::Window* window) : 
	mIsAlive(true), 
	mWindow(window), 
	mSprite(ResourceManager::getInstance().getTexture(textureFileName)),
	mTestSound1(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME1)),
	mTestSound2(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME2)),
	mTestSound3(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME3)),
	mTestSound4(ResourceManager::getInstance().getSoundBuffer(SOUND_FILENAME4))
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

void Luddis::updateMovement(){
	sf::Vector2f playerPosition(mSprite.getPosition());
	sf::Vector2f mousePosition(sf::Mouse::getPosition(*mWindow));
	sf::Vector2f direction = mousePosition - playerPosition;
	//Only move if not close to the cursor position
	if (VectorMath::getVectorLengthSq(direction) > graceArea){
		sf::Vector2f offset(VectorMath::normalizeVector(direction));
		mSprite.move(offset.x, offset.y);
	}
}

void Luddis::updateRotation(){
	sf::Vector2f playerPosition(mSprite.getPosition());
	playerPosition.x += mSprite.getTextureRect().width / 2;
	playerPosition.y += mSprite.getTextureRect().height / 2;
	sf::Vector2f mousePosition(sf::Mouse::getPosition(*mWindow));
	sf::Vector2f direction = mousePosition - playerPosition;
	float rotation = std::atan2f(direction.x, -direction.y) * 180 / (float)M_PI;
	mSprite.setRotation(rotation);
}

void Luddis::attack(){

}

void Luddis::handleInput(){
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true){
		updateMovement();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) == true){
		attack();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
		mTestSound1.play();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		mTestSound2.play();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
		mTestSound3.play();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		mTestSound4.play();
	}



	/*if (mTestSound1.getStatus() == sf::Sound::Playing){
		mTestSound1.stop();
	}*/
}