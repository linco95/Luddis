#define _USE_MATH_DEFINES
#include "LuddisStatePlayable.h"
#include "LuddisStateStunned.h"
#include "LuddisStateDead.h"
#include "Luddis.h"
#include "PowerupDisplay.h"
#include "SpiderWeb.h"
#include "Projectile.h"
#include "VectorMath.h"
#include "SoundEngine.h"
#include "Inventory.h"
#include "CollisionManager.h"
#include "CollidableEntity.h"
#include "EntityManager.h"
#include "Debug.h"
#include <array>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


// All of these should maybe be loaded from file instead, to avoid hard coded variables
//All float times are in seconds
static const float PROJECTILE_RELOAD = 0.4f;
static const float PROJECTILE_TIMER = 3.0f;
static const float GRACEAREA = 30;

#ifdef _DESIGNER_HAX_
static const float SUPERMOVESPEED = 800;
#endif //_DESIGNER_HAX_

static const int COLORVARIATIONS = 4;

static const float MOVESPEED = 200;

static const float PROJECTILE_SPEED = 300;
static const float MUZZLEOFFSET = 50.0f;
static const sf::Vector2f FRONTVECTOR(1, 0);
static const float STUNTIME = 3.0f;

static const std::string ANIMATION_FILEPATH[COLORVARIATIONS]{
	"Resources/Images/Spritesheets/Luddis_walkcykle",
	"Resources/Images/Spritesheets/Luddis_walkcykle_pink",
	"Resources/Images/Spritesheets/Luddis_walkcykle_blue",
	"Resources/Images/Spritesheets/Luddis_walkcykle_green"
};

static const std::string ANIMATION_ALMOSTDEAD[COLORVARIATIONS]{
	"Resources/Images/Spritesheets/luddis_CriticalHealth",
	"Resources/Images/Spritesheets/luddis_CriticalHealth_pink",
	"Resources/Images/Spritesheets/luddis_CriticalHealth_blue",
	"Resources/Images/Spritesheets/luddis_CriticalHealth_green"
};

static const std::string SHOT_ANIMATION[COLORVARIATIONS]{
	"Resources/Images/Spritesheets/Luddis_shot",
	"Resources/Images/Spritesheets/Luddis_shot_pink",
	"Resources/Images/Spritesheets/Luddis_shot_blue",
	"Resources/Images/Spritesheets/Luddis_shot_green"
};

static const std::string HIT_ANIMATION[COLORVARIATIONS]{
	"Resources/Images/Spritesheets/Luddis_hit",
	"Resources/Images/Spritesheets/Luddis_hit_pink",
	"Resources/Images/Spritesheets/Luddis_hit_blue",
	"Resources/Images/Spritesheets/Luddis_hit_green"
};

//This should be dynamic later to determine what texture to use for projectiles
static const std::array<std::string, 3> PROJECTILE_FILENAME = { "Resources/Images/Luddis_attack1.png",
"Resources/Images/Luddis_attack2.png",
"Resources/Images/Luddis_attack3.png"
};

LuddisStatePlayable::LuddisStatePlayable(Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display, sf::Shape* hitbox) :
	mProjectileCooldown(0),
	mPrevPos(0, 0),
	mIsFlipped(false),
	mMoved(false),
	mPlayerPtr(playerPtr),
	mEntityManager(entityManager),
	mWindow(window),
	mDisplay(display),
	mHurt(true),
	mHitbox(hitbox)
{
	Inventory::getInstance().choseFirst(new SpiderWeb(entityManager, display));
}

LuddisStatePlayable::~LuddisStatePlayable() {

}

void LuddisStatePlayable::tick(const sf::Time& deltaTime) {
	if (mProjectileCooldown >= 0) {
		mProjectileCooldown -= deltaTime.asSeconds();
	}

	handleInput(deltaTime);
	updateRotation();

	changeScale();

	if (Inventory::getInstance().getDust() == 0) {
		mPlayerPtr->setPlayerState(new LuddisStateDead(mPlayerPtr));
	}
}

#include <cassert>
void LuddisStatePlayable::collide(CollidableEntity * collidable, const sf::Vector2f& moveAway) {

	// Collision with solid object
	if (collidable->getCollisionCategory() == CollidableEntity::SOLID) {
		mPlayerPtr->move(moveAway);
	}
	// Collision with damaging object
	if (collidable->getCollisionCategory() == CollidableEntity::ENEMY_DAMAGE || collidable->getCollisionCategory() == CollidableEntity::ENEMY_DAMAGE_OBSTACLE) {
		mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION[mPlayerPtr->getColorScheme()]);
	}
	// Collision with a stunning entity
	if (collidable->getCollisionCategory() == CollidableEntity::ENEMY_STUN) {
		//Replace animation before changing state or a crash will occur.
		mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION[mPlayerPtr->getColorScheme()]);
		//TODO: add a way to make stun timers modular.
		mPlayerPtr->setPlayerState(new LuddisStateStunned(mPlayerPtr, 1.0f, mWindow, mEntityManager, mDisplay, mHitbox));
	}
}

sf::Vector2f LuddisStatePlayable::getVectorMouseToSprite() const {
	sf::Vector2f playerPosition(mPlayerPtr->getPosition());
	sf::Vector2f mousePosition(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
	return mousePosition - playerPosition;
}

void LuddisStatePlayable::handleInput(const sf::Time & deltaTime) {
	sf::Vector2f direction = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(direction) == 0) return;
	float rotation = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
	mPlayerPtr->setRotation(rotation);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		updateMovement(deltaTime);
	}
	else {

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		attack();
	}
	//Handle keyboard presses
	// TODO make this an event instead
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		Inventory::getInstance().activateFirst(sf::seconds(STUNTIME));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

	}
}

void LuddisStatePlayable::updateMovement(const sf::Time & deltaTime) {
	mDirectionVector = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(mDirectionVector) == 0) return;
	sf::Vector2f offset(VectorMath::normalizeVector(mDirectionVector));

	float moveX, moveY;

	moveX = offset.x*deltaTime.asSeconds()*MOVESPEED;
	moveY = offset.y*deltaTime.asSeconds()*MOVESPEED;

#ifdef _DESIGNER_HAX_
	moveX = offset.x*deltaTime.asSeconds()*SUPERMOVESPEED;
	moveY = offset.y*deltaTime.asSeconds()*SUPERMOVESPEED;
#endif // _DESIGNER_HAX_

	sf::Vector2f tempPos = mPlayerPtr->getPosition();

	//Only move if not too close to the cursor position
	if (VectorMath::getVectorLengthSq(mDirectionVector) > GRACEAREA) {
		mPlayerPtr->move(moveX, moveY);
		mPrevPos = -sf::Vector2f(moveX, moveY);
		mMoved = true;
	}
	else
		mMoved = false;
}

void LuddisStatePlayable::attack() {
	// If the projectile is on cooldown, return
	if (mProjectileCooldown > 0) return;
	// If not, shoot

	// Get the current direction of luddis based on his frontvector and rotation
	sf::Vector2f direction = VectorMath::rotateVector(FRONTVECTOR, mPlayerPtr->getRotation());

	// Replace the current animation with an shooting animation and play a shooting sound
	int percentDust = int((float(Inventory::getInstance().getDust()) / float(Inventory::getInstance().getMaxDust())) * 100);
	if (percentDust <= 10) {
		mPlayerPtr->getAnimation()->overrideAnimation(SHOT_ANIMATION[mPlayerPtr->getColorScheme()]);
	}
	else {
		mPlayerPtr->getAnimation()->overrideAnimation(SHOT_ANIMATION[mPlayerPtr->getColorScheme()]);
	}

	// TODO Pull out constant variable
	SoundEngine* se = &SoundEngine::getInstance();
	se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Shot");

	// Set the muzzlepoint where the projectile will get created
	sf::Vector2f muzzlePoint = mPlayerPtr->getPosition() + direction * MUZZLEOFFSET*mPlayerPtr->getScale().x;
	// Set the projectile cooldown
	mProjectileCooldown = PROJECTILE_RELOAD;

	// Choose a random projectile sprite
	int randValue = std::rand() % PROJECTILE_FILENAME.max_size();

	// create the projectile
	Projectile *proj = new Projectile(PROJECTILE_FILENAME[randValue], direction * PROJECTILE_SPEED, muzzlePoint, PROJECTILE_TIMER, CollidableEntity::Category::PLAYER_PROJECTILE);
	mEntityManager->addEntity(proj);
	CollisionManager::getInstance().addCollidable(proj);
}

void LuddisStatePlayable::updateRotation() {
	sf::Vector2f direction = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(direction) == 0) return;
	float rotation = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
	mPlayerPtr->setRotation(rotation);

	if ((direction.x <= 0 && !mIsFlipped) || (direction.x > 0 && mIsFlipped)) {
		mIsFlipped = !mIsFlipped;
	}
}

void LuddisStatePlayable::changeScale() {
	//int dust = Inventory::getInstance().getDust();
	//int max = Inventory::getInstance().getMaxDust();
	int percentDust = int((float(Inventory::getInstance().getDust()) / float(Inventory::getInstance().getMaxDust())) * 100);
	//Debug::log("Dust%" + std::to_string(percentDust));
	if (percentDust <= 10 && mHurt == false) {
		if (mScale != sf::Vector2f(1.0f, 1.0f)) {
			mScale = { 1.0f , 1.0f };
			mHitbox->setScale(mScale);
		}
		mPlayerPtr->getAnimation()->setDefaultAnimation(ANIMATION_ALMOSTDEAD[mPlayerPtr->getColorScheme()]);
		mHurt = true;
	}
	else if (percentDust <= 20 && percentDust > 10 && mHurt == true) {
		if (mScale != sf::Vector2f(1.0f, 1.0f)) {
			mScale = { 1.0f , 1.0f };
			mHitbox->setScale(mScale);
		}
		mPlayerPtr->getAnimation()->setDefaultAnimation(ANIMATION_FILEPATH[mPlayerPtr->getColorScheme()]);
		mHurt = false;
	}
	else if (percentDust <= 50 && percentDust > 20 && mScale != sf::Vector2f(1.2f, 1.2f)) {
		mScale = { 1.2f , 1.2f };
		mHitbox->setScale(mScale);
	}
	else if (percentDust <= 99 && percentDust > 50 && mScale != sf::Vector2f(1.3f, 1.3f)) {
		mScale = { 1.3f , 1.3f };
		mHitbox->setScale(mScale);
	}
	else if (percentDust <= 100 && percentDust > 99 && mScale != sf::Vector2f(1.4f, 1.4f)) {
		mScale = { 1.4f , 1.4f };
		mHitbox->setScale(mScale);
	}
	if (mIsFlipped == false)
		mPlayerPtr->setScale(mScale.y, mScale.y);
	if (mIsFlipped == true)
		mPlayerPtr->setScale(mScale.y, -mScale.y);

}

