#include "Collectible.h"
#include "ResourceManager.h"
#include "Inventory.h"
#include "SoundEngine.h"

static const Renderer::RenderLayer LAYER = Renderer::ITEM;
static const sf::CircleShape HITBOX_EGG_SHAPE = sf::CircleShape(10, 8);
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(15, 8);

Collectible::Collectible(sf::RenderWindow* window, const std::string& textureFilename, const sf::Vector2f& aPos, Collectible::CollectibleType type) :
	mIsAlive(true),
	mIsActive(true),
	mWindow(window),
	mType(type),
	//QUICKFIX!
	mAnimation("Resources/Images/Spritesheets/Spider_egg")
{
	if (mType == SPIDEREGG) {
		mHitbox = new sf::CircleShape(HITBOX_EGG_SHAPE);
		mAnimation = Animation(textureFilename);

		mAnimation.setOrigin(mAnimation.getSprite().getTextureRect().width / 2.f, mAnimation.getSprite().getTextureRect().height / 2.f);
		// Set the animation to start at a random frame. Should be read from the animation TODO
		mAnimation.setFrame(std::rand() % 4);
	}
	else {
		mSprite = sf::Sprite(ResourceManager::getInstance().getTexture(textureFilename));
		mHitbox = new sf::CircleShape(HITBOX_SHAPE);

		mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);		
	}
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
	setPosition(aPos);
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
}

Collectible::~Collectible() {
	delete mHitbox;
}

void Collectible::tick(const sf::Time& deltaTime) {
	mAnimation.tick(deltaTime);
}

void Collectible::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	if (mType == SPIDEREGG) {
		target.draw(mAnimation, states);
	}
	else target.draw(mSprite, states);
}

bool Collectible::isAlive() const {
	return mIsAlive;
}

bool Collectible::isActive() const {
	return mIsActive;
}

void Collectible::setActive(const bool& active) {
	mIsActive = active;
}

Renderer::RenderLayer Collectible::getRenderLayer() const {
	return LAYER;
}

sf::FloatRect Collectible::getHitBox() {
	if (mType == SPIDEREGG) {
		return getTransform().transformRect(mAnimation.getSprite().getGlobalBounds());
	}
	else {
		return getTransform().transformRect(mSprite.getGlobalBounds());
	}
}

sf::Shape* Collectible::getNarrowHitbox() const {
	return mHitbox;
}
int Collectible::getCollisionDamage() const{
	return 0;
}
Collectible::Category Collectible::getCollisionCategory() {
	return COLLECT;
}

Collectible::Type Collectible::getCollisionType() {
	return CIRCLE;
}

void Collectible::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
		mIsAlive = false;
		SoundEngine* se = &SoundEngine::getInstance();
		switch (mType) {
		case DUST:
			Inventory::getInstance().addDust(1);

			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Dust");
			break;
		case CHIPS:
			Inventory::getInstance().changeChips(1);
			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Crumb");
			break;
		case SPIDEREGG:
			Inventory::getInstance().changeEggs(1);
			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Dust");
			break;
		case POWERUP:
			break;
		default:
			break;
		}
	}
}

void Collectible::stun(const sf::Time& deltatime) {
	return;
}