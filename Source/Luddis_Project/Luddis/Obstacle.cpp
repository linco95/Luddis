#include "Obstacle.h"
#include "ResourceManager.h"

const Entity::RenderLayer LAYER = Entity::RenderLayer::OBSTACLES;
const int DAMAGE = 0;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Obstacle::Obstacle(std::string textureFilename, sf::RenderWindow* window, ObstacleType type):
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mType(type),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	setPosition(150, 150);
}

Obstacle::~Obstacle(){
	delete mHitbox;
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

sf::Shape* Obstacle::getNarrowHitbox() const{
	return mHitbox;
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
