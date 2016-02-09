#include "PowerupDisplay.h"
#include "ResourceManager.h"

static const sf::Vector2f RECT_SIZE(36, 36);

PowerupDisplay::PowerupDisplay(std::string textureFilename, sf::Vector2f pos, float cooldown) :
mIsAlive(true),
mIsActive(true),
MAX_COOLDOWN(cooldown),
mCooldown(0.0f),
mRectShape(RECT_SIZE),
mSprite(ResourceManager::getInstance().getTexture(textureFilename)){
	setPosition(pos);
	setOrigin((float)mSprite.getTextureRect().width / 2, (float)mSprite.getTextureRect().height / 2);
	sf::Color color(100, 100, 100, 150);
	mRectShape.setFillColor(color);
	mRectShape.setOutlineColor(color);
	mRectShape.move(5.0f, 5.0f);
}

PowerupDisplay::~PowerupDisplay(){

}

void PowerupDisplay::tick(const sf::Time& deltaTime){
	if (mCooldown > 0){
		mCooldown -= deltaTime.asSeconds();
	}
	updateCooldown(deltaTime);
}

void PowerupDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mRectShape, states);
}

bool PowerupDisplay::isAlive() const{
	return mIsAlive;
}

bool PowerupDisplay::isActive() const{
	return mIsActive;
}

void PowerupDisplay::setActive(const bool& active){
	mIsActive = active;
}

PowerupDisplay::RenderLayer PowerupDisplay::getRenderLayer() const{
	return GUI_FOREGROUND;
}

float PowerupDisplay::getCooldown() const{
	return mCooldown;
}

void PowerupDisplay::activateCooldown(){
	if (mCooldown <= 0){
		mCooldown = MAX_COOLDOWN;
	}
}

void PowerupDisplay::updateCooldown(const sf::Time& deltaTime){
	sf::Vector2f size(RECT_SIZE.x, RECT_SIZE.y*(mCooldown / MAX_COOLDOWN));
	mRectShape.setSize(size);
}