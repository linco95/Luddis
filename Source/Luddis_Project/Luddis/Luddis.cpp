#include "Luddis.h"
#include "Inventory.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "GameStateLevel.h"
#include "GameManager.h"
#include "GUIManager.h"
#include "CollisionManager.h"
#include "SoundEngine.h"
#include "VectorMath.h"
#include "LuddisStatePlayable.h"
#include "Projectile.h"
#include "Dialogue.h"
#include "ViewUtility.h"
#include <SFML/System.hpp>
#include <cmath>
#include <string>
#include <array>

static const int COLORVARIATIONS = 4;

static const std::string ANIMATION_FILEPATH[COLORVARIATIONS]{ 
	"Resources/Images/Spritesheets/Luddis_walkcykle",
	"Resources/Images/Spritesheets/Luddis_walkcykle_pink",
	"Resources/Images/Spritesheets/Luddis_walkcykle_blue",
	"Resources/Images/Spritesheets/Luddis_walkcykle_green"
};

static const sf::Vector2f FRONTVECTOR(1, 0);

static const Renderer::RenderLayer LAYER = Renderer::PLAYER;
static const sf::CircleShape HITBOX_SHAPE = sf::CircleShape(35, 8);

//TEMP
static Animation STASISANIMATION("Resources/Images/Spritesheets/Stasis_ring");


Luddis::Luddis(sf::RenderWindow* window, EntityManager* entityManager) :
	mIsAlive(true),
	mIsActive(true),
	mWindow(window),
	mEntityManager(entityManager),
	mAnimation(ANIMATION_FILEPATH[0]),
	mHitbox(new sf::CircleShape(HITBOX_SHAPE)),
	mLife(Inventory::getInstance().getDust())
{
	setPosition(mWindow->getView().getSize().x / 2, mWindow->getView().getSize().y / 2);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
}

Luddis::~Luddis() {
	delete mHitbox;
}

bool Luddis::isAlive() const {
	return mIsAlive;
}

bool Luddis::isActive() const {
	return mIsActive;
}

void Luddis::setActive(const bool& active) {
	mIsActive = active;
}

void Luddis::tick(const sf::Time& deltaTime) {
	//Update the currently active state, unless null pointer.
	if (mCurrentLuddState != nullptr)
		mCurrentLuddState->tick(deltaTime);


	mAnimation.tick(deltaTime);

	// Temporary
	static const float ROTATIONSPEED = 75.f;
	static int x = 0;
	static const float interval = 1.f;
	static const float pulseSpeed = 0.5f;

	STASISANIMATION.setScale(getScale() + sf::Vector2f(1, 1) * interval * ((float)std::sin(x * pulseSpeed) + 0.5f) * deltaTime.asSeconds());
	STASISANIMATION.setPosition(getPosition());
	STASISANIMATION.rotate(ROTATIONSPEED * deltaTime.asSeconds());
	STASISANIMATION.tick(deltaTime);
	x++;
	/*(x >= 10) ? x = 0 : x++;*/
	// Update scale

}

void Luddis::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(STASISANIMATION, states);
	states.transform *= getTransform();
	target.draw(mAccessoryTail, states);
	target.draw(mAnimation.getCurrAnimation(), states);
	target.draw(mAccessoryHead, states);
}

Luddis::Category Luddis::getCollisionCategory() {
	return PLAYER_OBJECT;
}

Luddis::Type Luddis::getCollisionType() {
	return REC;
}

void Luddis::collide(CollidableEntity *collidable, const sf::Vector2f& moveAway) {
	if (mCurrentLuddState != nullptr)
		mCurrentLuddState->collide(collidable, moveAway);

}

void Luddis::setPlayerState(LuddisState * luddisState) {
	//Right now each state handles the creation of new states.
	//Perhaps it would be better if Luddis handles it instead?
	//It would save having to pass on useless parameters, and also
	//reduce dependence between the states. Less coupling.
	delete mCurrentLuddState;
	mCurrentLuddState = luddisState;
}

sf::FloatRect Luddis::getHitBox() {
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}

sf::Shape* Luddis::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	return mHitbox;
}

int Luddis::getCollisionDamage() const{
	return 0;
}

Renderer::RenderLayer Luddis::getRenderLayer() const {
	return LAYER;
}

void Luddis::reset(GameStateLevel* gameStateLevel) {
	//TODO: Make this create a gameover menu 
	mGameStateLevel->resetLevel();
	GameManager::getInstance().setGameState(mGameStateLevel);
}

AnimationQueue* Luddis::getAnimation() {
	return &mAnimation;
}

void Luddis::stun(const sf::Time& deltatime) {

}

void Luddis::setAccessoryHead(std::string filename) {
	if (filename != "") {
		mAccessoryHead.setTexture(ResourceManager::getInstance().getTexture(filename));
		mAccessoryHead.setOrigin(mAccessoryHead.getGlobalBounds().width / 2, mAccessoryHead.getGlobalBounds().height / 2);
	}
}

void Luddis::setAccessoryTail(std::string filename) {
	if (filename != "") {
		mAccessoryTail.setTexture(ResourceManager::getInstance().getTexture(filename));
		mAccessoryTail.setOrigin(mAccessoryTail.getGlobalBounds().width / 2, mAccessoryTail.getGlobalBounds().height / 2);
	}
}

void Luddis::setColorScheme(int index) {
	//Do derpy things
	if (index >= 0 && index < COLORVARIATIONS) {
		mColorScheme = index;
		mAnimation.setDefaultAnimation(ANIMATION_FILEPATH[mColorScheme]);
	}
}

int Luddis::getColorScheme() const{
	return mColorScheme;
}
