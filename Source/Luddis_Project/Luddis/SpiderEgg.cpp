#include "SpiderEgg.h"
#include "ResourceManager.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;
static const sf::CircleShape HITBOX_SHAPE(10, 8);

SpiderEgg::SpiderEgg(std::string textureFilename, sf::RenderWindow* window) :
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
}

SpiderEgg::~SpiderEgg(){
	delete mHitbox;
}

void SpiderEgg::tick(const sf::Time& deltaTime){

}

void SpiderEgg::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool SpiderEgg::isAlive(){
	return mIsAlive;
}

Entity::RenderLayer SpiderEgg::getRenderLayer() const{
	return LAYER;
}

sf::FloatRect SpiderEgg::getHitBox(){
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* SpiderEgg::getNarrowHitbox() const{
	return mHitbox;
}
SpiderEgg::Category SpiderEgg::getCollisionCategory(){
	return COLLECT;
}

SpiderEgg::Type SpiderEgg::getCollisionType(){
	return CIRCLE;
}

void SpiderEgg::collide(CollidableEntity *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
		Inventory::getInstance().changeEggs(1);
	}
}