#include "Luddis.h"
#include "ResourceManager.h"
#include "VectorMath.h"
#include <string>


Luddis::Luddis(std::string textureFileName, sf::Window* window){
	mSprite.setTexture(ResourceManager::getInstance().getTexture(textureFileName));
	mWindow = window;
}

Luddis::~Luddis(){

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
