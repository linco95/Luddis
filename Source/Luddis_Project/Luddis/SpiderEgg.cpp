#include "SpiderEgg.h"
#include "ResourceManager.h"
#include "Inventory.h"

static const Entity::RenderLayer LAYER = Entity::RenderLayer::ITEM;
static const sf::CircleShape HITBOX_SHAPE(10, 8);

SpiderEgg::SpiderEgg(sf::RenderWindow* window, const std::string& textureFilename, const sf::Vector2f& aPos) :
mIsAlive(true),
mIsActive(true),
mWindow(window),
mAnimation(textureFilename),
mHitbox(new sf::CircleShape(HITBOX_SHAPE))
{
	mAnimation.setOrigin(mAnimation.getSprite().getTextureRect().width / 2.f, mAnimation.getSprite().getTextureRect().height / 2.f);
	// Set the animation to start at a random frame. Should be read from the animation TODO
	mAnimation.setFrame(std::rand() % 4);
	setPosition(aPos);
	mHitbox->setPosition(getPosition());
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
}

SpiderEgg::~SpiderEgg(){
	delete mHitbox;
}

void SpiderEgg::tick(const sf::Time& deltaTime){
	mAnimation.tick(deltaTime);
}

void SpiderEgg::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mAnimation, states);
}

bool SpiderEgg::isAlive() const{
	return mIsAlive;
}

bool SpiderEgg::isActive() const{
	return mIsActive;
}

void SpiderEgg::setActive(const bool& active){
	mIsActive = active;
}

Entity::RenderLayer SpiderEgg::getRenderLayer() const{
	return LAYER;
}

sf::FloatRect SpiderEgg::getHitBox(){
	return getTransform().transformRect(mAnimation.getSprite().getGlobalBounds());
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

void SpiderEgg::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway){
	if (collidable->getCollisionCategory() == PLAYER_OBJECT){
		mIsAlive = false;
		Inventory::getInstance().changeEggs(1);
	}
}

void SpiderEgg::stun(const sf::Time& deltatime) {
	return;
}