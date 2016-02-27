#include "Dust.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Dust::Dust(sf::RenderWindow* window, std::string textureFilename, const sf::Vector2f& position, const float& angle) :
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

Dust::~Dust(){
	delete mHitbox;
}

void Dust::tick(const sf::Time& deltaTime){
}


void Dust::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Dust::isAlive() const{
	return mIsAlive;
}

bool Dust::isActive() const{
	return mIsActive;
}

void Dust::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer Dust::getRenderLayer() const{
	return LAYER;
}

Dust::Category Dust::getCollisionCategory(){
	return COLLECT;
}

Dust::Type Dust::getCollisionType(){
	return REC;
}

void Dust::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		Inventory::getInstance().addDust(1);
		SoundEngine::getInstance().playSound("resources/audio/Damm_Slurp_01.wav");
	}
}

sf::FloatRect Dust::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}
sf::Shape* Dust::getNarrowHitbox() const{
	return mHitbox;
}
