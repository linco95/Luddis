#include "Silverfish.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <string>

static const float speed = 0;

Silverfish::Silverfish(std::string textureFilename, sf::Window* window) :
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
}

Silverfish::~Silverfish(){

}

void Silverfish::tick(const sf::Time& deltaTime){

}

void Silverfish::draw(sf::RenderTarget& target, sf::RenderStates states) const{

}

bool Silverfish::isAlive() const{
	return mIsAlive;
}
