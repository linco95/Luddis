#include "SpiderEgg.h"
#include "ResourceManager.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;

SpiderEgg::SpiderEgg(std::string textureFilename, sf::RenderWindow* window):
mIsAlive(true),
mWindow(window),
mSprite(ResourceManager::getInstance().getTexture(textureFilename))
{
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
}

SpiderEgg::~SpiderEgg(){
	Inventory::getInstance().changeEggs(1);
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

Collidable::Category SpiderEgg::getCollisionCategory(){
	return COLLECT;
}

Collidable::Type SpiderEgg::getCollisionType(){
	return CIRCLE;
}

void SpiderEgg::collide(Collidable *collidable){
	if (collidable->getCollisionCategory() == FRIEND){
		mIsAlive = false;
	}
}