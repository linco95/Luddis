#include "Mannequin.h"
#include "ViewUtility.h"
#include <SFML/Graphics/Texture.hpp>
#include "ResourceManager.h"

static const char* CHARACTER_TEXTURE = "Resources/Images/Spritesheets/Luddis_walkcykle";
static const char* CHARACTER_TEXTURE_PINK = "Resources/Images/Spritesheets/Luddis_walkcykle_pink";
static const char* CHARACTER_TEXTURE_BLUE = "Resources/Images/Spritesheets/Luddis_walkcykle_blue";
static const char* CHARACTER_TEXTURE_GREEN = "Resources/Images/Spritesheets/Luddis_walkcykle_green";

Mannequin::Mannequin() :
	mIsActive(true),
	mIsAlive(true),
	mAnimate(true),
	mStatic(false),
	mStrata(FOURTH),
	mCurrentHeadImage(""),
	mCurrentTailImage(""),
	mAnimation{ (CHARACTER_TEXTURE), (CHARACTER_TEXTURE_PINK), (CHARACTER_TEXTURE_BLUE), (CHARACTER_TEXTURE_GREEN) },
	mColor(ColorScheme::DEFAULT){

	mHeadAccessory.setPosition(getPosition());
	mTailAccessory.setPosition(getPosition());
}

Mannequin::Mannequin(std::string filename) :
	Mannequin() {

	mStaticPortrait.setTexture(ResourceManager::getInstance().getTexture(filename));
	mStatic = true;

}

Mannequin::~Mannequin() {

}

void Mannequin::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	if (mStatic)
		target.draw(mStaticPortrait);
	else {
		if (mShowTailAccessory)
			target.draw(mTailAccessory);
		target.draw(mAnimation[mColor], states);
		if (mShowHeadAccessory)
			target.draw(mHeadAccessory);
	}
}

void Mannequin::tick(const sf::Time& deltaTime) {
	if (mAnimate) {
		for (int i = 0; i < 4; i++) {
			mAnimation[i].tick(deltaTime);
		}
	}
}

Mannequin::Strata Mannequin::getRenderLayer() const {
	return mStrata;
}

void Mannequin::setRenderLayer(Strata strata) {
	mStrata = strata;
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

void Mannequin::setPosition(float x, float y) {
	for (int i = 0; i < COLORVARIATIONS; i++)
		mAnimation[i].setPosition(x, y);
	mTailAccessory.setPosition(x, y);
	mHeadAccessory.setPosition(x, y);
	mStaticPortrait.setPosition(x, y);
}

void Mannequin::setPosition(const sf::Vector2f & position) {
	float x = position.x, y = position.y;
	setPosition(x, y);
}

void Mannequin::setScale(float x, float y) {
	for (int i = 0; i < COLORVARIATIONS; i++)
		mAnimation[i].setScale(x, y);
	mTailAccessory.setScale(x, y);
	mHeadAccessory.setScale(x, y);
	mStaticPortrait.setScale(x, y);
}

void Mannequin::setScale(const sf::Vector2f & scale) {
	float x = scale.x, y = scale.y;
	setScale(x, y);
}

void Mannequin::setAnimate(bool animate) {
	mAnimate = animate;
}

void Mannequin::setHeadAccessory(const char* filename) {
	mCurrentHeadImage = filename;
	if (filename == nullptr || filename == "")
		mShowHeadAccessory = false;
	else {
		mShowHeadAccessory = true;
		mHeadAccessory.setTexture(ResourceManager::getInstance().getTexture(filename));
		mHeadAccessory.setOrigin(mHeadAccessory.getLocalBounds().width*0.5f, mHeadAccessory.getLocalBounds().height*0.5f);
	}
}

std::string Mannequin::getHeadAccessory() const {
	return mCurrentHeadImage;
}

void Mannequin::setTailAccessory(const char* filename) {
	mCurrentTailImage = filename;
	if (mCurrentTailImage == "")
		mShowTailAccessory = false;
	else {
		mShowTailAccessory = true;
		mTailAccessory.setTexture(ResourceManager::getInstance().getTexture(filename));
		mTailAccessory.setOrigin(mTailAccessory.getLocalBounds().width*0.5f, mTailAccessory.getLocalBounds().height*0.5f);
	}
}

std::string Mannequin::getTailAccessory() const {
	return mCurrentTailImage;
}

void Mannequin::setColorScheme(ColorScheme colorScheme) {
	mColor = colorScheme;
}

Mannequin::ColorScheme Mannequin::getColorScheme() const {
	return mColor;
}

const sf::Sprite* Mannequin::getSprite() const {
	return &mAnimation[mColor].getSprite();
}

void Mannequin::reset() {
	mShowHeadAccessory = false;
	mShowTailAccessory = false;
	mColor = Mannequin::ColorScheme::DEFAULT;
}
