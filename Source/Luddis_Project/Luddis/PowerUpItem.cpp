#include "PowerUpItem.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

PowerUpItem::PowerUpItem(std::string textureFilename, const sf::Vector2f& position) :
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

PowerUpItem::~PowerUpItem(){
	delete mHitbox;
}

void PowerUpItem::tick(const sf::Time& deltaTime){
}


void PowerUpItem::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool PowerUpItem::isAlive() const{
	return mIsAlive;
}

bool PowerUpItem::isActive() const{
	return mIsActive;
}

void PowerUpItem::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer PowerUpItem::getRenderLayer() const{
	return LAYER;
}

PowerUpItem::Category PowerUpItem::getCollisionCategory(){
	return COLLECT;
}

PowerUpItem::Type PowerUpItem::getCollisionType(){
	return REC;
}


void PowerUpItem::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		Inventory::getInstance().changePowerUp(1);
	}
}

sf::FloatRect PowerUpItem::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* PowerUpItem::getNarrowHitbox() const{
	return mHitbox;
}

void PowerUpItem::stun(const sf::Time& deltatime) {
	return;
}