#include "Mannequin.h"
#include "ViewUtility.h"
#include "ResourceManager.h"

static const char* CHARACTER_TEXTURE = "Resources/Images/Spritesheets/Luddis_walkcykle";

Mannequin::Mannequin() :
	mIsActive(true),
	mIsAlive(true),
	mAnimate(true),
	mAnimation(CHARACTER_TEXTURE) {
	setScale(-3.0f, 3.0f);
	sf::Vector2f position(ViewUtility::getViewSize().getCenter());
	setPosition(position);
}

Mannequin::~Mannequin() {

}

void Mannequin::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mTailAccessory);
	target.draw(mAnimation, states);
	target.draw(mHeadAccessory);
}

void Mannequin::tick(const sf::Time& deltaTime) {
	if (mAnimate) {
		mAnimation.tick(deltaTime);
	}
}

Mannequin::Strata Mannequin::getRenderLayer() const {
	return FOURTH;
}

bool Mannequin::isAlive() const {
	return mIsAlive;
}

bool Mannequin::isActive() const {
	return mIsActive;
}

void Mannequin::setActive(const bool & active) {
	mIsActive = active;
}

void Mannequin::setAnimate(bool animate) {
	mAnimate = animate;
}

void Mannequin::setHeadAccessory(const char * filename) {
	mHeadAccessory.setTexture(ResourceManager::getInstance().getTexture(filename));
}

void Mannequin::setTailAccessory(const char * filename) {
	mTailAccessory.setTexture(ResourceManager::getInstance().getTexture(filename));
}
