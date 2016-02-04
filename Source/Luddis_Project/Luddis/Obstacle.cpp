#include "Obstacle.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;

Obstacle::Obstacle(std::string textureFilename, sf::RenderWindow* window, ObstacleType type):
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mType(type)
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	setPosition(150, 150);
}

Obstacle::~Obstacle(){
}

void Obstacle::tick(const sf::Time& deltaTime){
	return;
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Obstacle::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer Obstacle::getRenderLayer() const{
	return LAYER;
}

sf::FloatRect Obstacle::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

Collidable::Category Obstacle::getCollisionCategory(){
	if (mType == DAMAGE){
		return BG_DAMAGE;
	}
	else /*if (mType == SOLID)*/{
		return BG_SOLID;
	}
}

Collidable::Type Obstacle::getCollisionType(){
	return REC;
}

void Obstacle::collide(Collidable *collidable){
}
