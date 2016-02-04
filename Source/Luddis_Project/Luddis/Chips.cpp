#include "Chips.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include "SoundEngine.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Chips::Chips(std::string textureFilename, sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	// Get a y-spawn position
	sf::Vector2f size = mWindow->getView().getSize();
	int r1 = rand() % (int)size.y + 1;
	int r2 = rand() % (int)size.x + 1;
	// Set spawn position
	mSprite.setPosition((float)r2, (float)r1);

}

Chips::~Chips(){
	// Samma som pEäggen
	Inventory::getInstance().changeChips(1);
	delete mHitbox;
}

void Chips::tick(const sf::Time& deltaTime){
}


void Chips::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Chips::isAlive(){
	return mIsAlive;
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


void Chips::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		
		SoundEngine::getInstance().playSound("resources/audio/luddis_crumbgather_s1d2v1.wav");
	}
}

sf::FloatRect Chips::getHitBox(){
	return mSprite.getGlobalBounds();
}

sf::Shape* Chips::getNarrowHitbox() const{
	return mHitbox;
}