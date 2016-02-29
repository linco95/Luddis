#include "Chips.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Chips::Chips(sf::RenderWindow* window, std::string textureFilename, const sf::Vector2f& position, const float& angle) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Set spawn position
	setPosition(position);
	rotate(angle);

	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);

	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
}

Chips::~Chips(){
	delete mHitbox;
}

void Chips::tick(const sf::Time& deltaTime){
}


void Chips::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Chips::isAlive() const{
	return mIsAlive;
}

bool Chips::isActive() const{
	return mIsActive;
}

void Chips::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Chips::getRenderLayer() const{
	return LAYER;
}

Chips::Category Chips::getCollisionCategory(){
	return COLLECT;
}

Chips::Type Chips::getCollisionType(){
	return REC;
}


void Chips::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		Inventory::getInstance().changeChips(1);
		SoundEngine::getInstance().playSound("resources/audio/luddis_crumbgather_s1d2v1.wav");
	}
}

sf::FloatRect Chips::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* Chips::getNarrowHitbox() const{
	return mHitbox;
}

void Chips::stun(const sf::Time& deltatime) {
	return;
}