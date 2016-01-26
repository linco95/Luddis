#include "Luddis.h"
#include "ResourceManager.h"
#include "VectorMath.h"
#include <string>


Luddis::Luddis(std::string textureFileName, sf::Window* window) : 
	mIsAlive(true), 
	mWindow(window), 
	mSprite(ResourceManager::getInstance().getTexture(textureFileName)) { }

Luddis::~Luddis(){

}
bool Luddis::isAlive() const{
	return mIsAlive;
}
void Luddis::tick(const sf::Time& deltaTime){
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true){
		sf::Vector2f playerPosition(mSprite.getPosition());
		sf::Vector2f mousePosition(sf::Mouse::getPosition(*mWindow));
		sf::Vector2f direction = mousePosition - playerPosition;
		sf::Vector2f offset(VectorMath::normalizeVector(direction));
		mSprite.move(offset.x, offset.y);
	}
}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	
	states.transform *= getTransform();
	
	target.draw(mSprite, states);
}
