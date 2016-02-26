#define _USE_MATH_DEFINES
#include "BackgroundEffect.h"
#include "ResourceManager.h"
#include <cmath>

static const Entity::RenderLayer LAYER = Entity::RenderLayer::PLAYER;
static const float ROTATIONSPEED = 0.5f;
static const float FADINGFACTOR = 0.5f;


//The max life time should be entered in milliseconds
BackgroundEffect::BackgroundEffect(std::string textureFilename, sf::Vector2f direction, sf::Vector2f position, float maxLifeTimeMS, Transformable* aTarget) :
	mIsAlive(true),
	mIsActive(false),
	mDirection(direction),
	mLifeTime(maxLifeTimeMS),
	mSprite(ResourceManager::getInstance().getTexture(textureFilename)),
	mTarget(aTarget)
{
	mSprite.setRotation((float)(rand() % 360));
	mSprite.setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	setPosition(position);
}

BackgroundEffect::~BackgroundEffect() {
}

void fadeBackgroundEffect(sf::Sprite& a_Sprite, const sf::Time& a_TimeLeft) {
	sf::Color curColor = a_Sprite.getColor();
	curColor.a *= 1; // fadingfactor / timeleft -ish
}

void BackgroundEffect::tick(const sf::Time& deltaTime) {
	if (getPosition().x <= (mTarget->getPosition().x + 1000) && getPosition().x >= (mTarget->getPosition().x - 1000)) {
		mIsActive = true;
	}
	if (!mIsActive) return;
	mLifeTime -= deltaTime.asSeconds();
	checkLifeTime();
	updateMovement(deltaTime);
	rotate(ROTATIONSPEED);
}

void BackgroundEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool BackgroundEffect::isAlive() const {
	return mIsAlive;
}

bool BackgroundEffect::isActive() const {
	return mIsActive;
}

void BackgroundEffect::setActive(const bool& active) {
	mIsActive = active;
}

void BackgroundEffect::updateMovement(const sf::Time& deltaTime) {
	move(mDirection*deltaTime.asSeconds());
}

void BackgroundEffect::checkLifeTime() {
	if (mLifeTime <= 0) {
		mIsAlive = false;
	}
}

Entity::RenderLayer BackgroundEffect::getRenderLayer() const {
	return LAYER;
}

void BackgroundEffect::setTexture(std::string filename) {
	mSprite.setTexture(ResourceManager::getInstance().getTexture(filename), true);
}

