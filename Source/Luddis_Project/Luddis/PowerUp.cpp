#include "PowerUp.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

PowerUp::PowerUp(std::string textureFilename, const sf::Vector2f& position) :
mIsAlive(true),
mIsActive(true),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Set spawn position
	setPosition(position);

	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
}

PowerUp::~PowerUp(){
	delete mHitbox;
}

void PowerUp::tick(const sf::Time& deltaTime){
}


void PowerUp::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool PowerUp::isAlive() const{
	return mIsAlive;
}

bool PowerUp::isActive() const{
	return mIsActive;
}

void PowerUp::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer PowerUp::getRenderLayer() const{
	return LAYER;
}

PowerUp::Category PowerUp::getCollisionCategory(){
	return COLLECT;
}

PowerUp::Type PowerUp::getCollisionType(){
	return REC;
}


void PowerUp::collide(CollidableEntity *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		Inventory::getInstance().changePowerUp(1);
	}
}

sf::FloatRect PowerUp::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* PowerUp::getNarrowHitbox() const{
	return mHitbox;
}
