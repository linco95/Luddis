#include "Dishrag.h"
#include "ResourceManager.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;

Dishrag::Dishrag(std::string textureFilename, sf::RenderWindow* window, sf::Vector2f position) :
	mIsAlive(true),
	mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	mSprite.setPosition(position);
}

Dishrag::~Dishrag(){
	//TODO
	//Trigger end of course 1
}

void Dishrag::tick(const sf::Time& deltaTime){
	//TODO
	//Implement update
}

void Dishrag::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Dishrag::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer Dishrag::getRenderLayer() const{
	return LAYER;
}

sf::FloatRect Dishrag::getHitBox(){
	return mSprite.getGlobalBounds();
}

Dishrag::Category Dishrag::getCollisionCategory(){
	return ENEMY;
}

Dishrag::Type Dishrag::getCollisionType(){
	return REC;
}

void Dishrag::collide(Collidable *collidable){
	//TODO
	//Implement boss damage and effect of damage
}