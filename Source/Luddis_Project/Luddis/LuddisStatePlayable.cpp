#define _USE_MATH_DEFINES
#include "LuddisStatePlayable.h"
#include "LuddisStateStunned.h"
#include "LuddisStateDead.h"
#include "Luddis.h"
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
#include "PowerupDisplay.h"
#include "SpiderWeb.h"


// All of these should maybe be loaded from file instead, to avoid hard coded variables
//All float times are in seconds
static const float PROJECTILE_RELOAD = 0.4f;
static const float PROJECTILE_TIMER = 3.0f;
static const float INVINCIBILITY_TIMER = 0.75f;
static const float GRACEAREA = 30;

#ifdef _DESIGNER_HAX_
static const float SUPERMOVESPEED = 300;
#endif //_DESIGNER_HAX_

static const float MOVESPEED = 200;

static const float PROJECTILE_SPEED = 300;
static const float MUZZLEOFFSET = 50.0f;
static const sf::Vector2f FRONTVECTOR(1, 0);
static const float STUNTIME = 3.0f;

static const std::string ANIMATION_FILEPATH = "Resources/Images/Spritesheets/Luddis_walkcykle";
static const std::string ANIMATION_ALMOSTDEAD = "Resources/Images/Spritesheets/luddis_CriticalHealth";

static const std::string SHOT_ANIMATION = "Resources/Images/Spritesheets/Luddis_shot";
static const std::string HIT_ANIMATION = "Resources/Images/Spritesheets/Luddis_hit";
static const std::string SOUND_FILENAME1 = "Resources/Audio/Skott_Blås_Små_01.wav";
static const std::string SOUND_FILENAME2 = "Resources/Audio/Skott_Blås_Små_02.wav";
static const std::string SOUND_FILENAME3 = "Resources/Audio/Skott_Blås_Små_03.wav";

//This should be dynamic later to determine what texture to use for projectiles
static const std::array<std::string, 3> PROJECTILE_FILENAME = { "Resources/Images/Luddis_attack1.png",
"Resources/Images/Luddis_attack2.png",
"Resources/Images/Luddis_attack3.png"
};

LuddisStatePlayable::LuddisStatePlayable(Luddis* playerPtr, sf::RenderWindow* window, EntityManager* entityManager, PowerupDisplay* display) :
	mProjectileCooldown(0),
	mInvincibility(INVINCIBILITY_TIMER),
	mPrevPos(0, 0),
	mIsFlipped(false),
	mMoved(false),
	mPlayerPtr(playerPtr),
	mEntityManager(entityManager),
	mWindow(window),
	mDisplay(display)
{
	Inventory::getInstance().choseFirst(new SpiderWeb(entityManager, display));
}

LuddisStatePlayable::~LuddisStatePlayable(){

}

void LuddisStatePlayable::tick(const sf::Time& deltaTime){
	if (mProjectileCooldown >= 0) {
		mProjectileCooldown -= deltaTime.asSeconds();
	}
	if (mInvincibility >= 0) {
		mInvincibility -= deltaTime.asSeconds();
	}

	handleInput(deltaTime);
	updateRotation();

	changeScale();
}
#include <cassert>
void LuddisStatePlayable::collide(CollidableEntity * collidable, const sf::Vector2f& moveAway) {

	// Collision with solid object
	if (collidable->getCollisionCategory() == CollidableEntity::SOLID) {
		//mColliding = true;
		//mCollideBox = collidable->getHitBox();
		//Debug::log("MoveAway x: " + std::to_string(moveAway.x) + ". y:" + std::to_string(moveAway.y), Debug::INFO);
		mPlayerPtr->move(moveAway);
	}
	if (mInvincibility <= 0) {
		// Collision with damaging object
		if (collidable->getCollisionCategory() == CollidableEntity::ENEMY_DAMAGE) {
			mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION);
			mInvincibility += INVINCIBILITY_TIMER;
			if (Inventory::getInstance().getDust() == 0) {
				mPlayerPtr->setPlayerState(new LuddisStateDead(mPlayerPtr));
			}
			Inventory::getInstance().addDust(-1);
		}
		// Collision with a collectible
		/*if (collidable->getCollisionCategory() == COLLECT){
		// NO-OP
		}*/
		// Collision with a stunning entity
		if (collidable->getCollisionCategory() == CollidableEntity::ENEMY_STUN) {
			//Replace animation before changing state or a crash will occur.
			mPlayerPtr->getAnimation()->replaceAnimation(HIT_ANIMATION);
			//TODO: add a way to make stun timers modular.
			mPlayerPtr->setPlayerState(new LuddisStateStunned(mPlayerPtr, 1.0f, mWindow, mEntityManager, mDisplay));
			mInvincibility += INVINCIBILITY_TIMER;
		}
	}
}

sf::Vector2f LuddisStatePlayable::getVectorMouseToSprite() const {
	sf::Vector2f playerPosition(mPlayerPtr->getPosition());
	sf::Vector2f mousePosition(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
	return mousePosition - playerPosition;
}

void LuddisStatePlayable::handleInput(const sf::Time & deltaTime){
	sf::Vector2f direction = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(direction) == 0) return;
	float rotation = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
	mPlayerPtr->setRotation(rotation);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		updateMovement(deltaTime);
	}
	else {
		mMoved = false;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		attack();
	}
	//Handle keyboard presses
	// TODO make this an event instead
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		if (true) {
			Inventory::getInstance().activateFirst(sf::seconds(STUNTIME));
		}
		else {
			return;
		}
	}
}

void LuddisStatePlayable::updateMovement(const sf::Time & deltaTime) {
	mDirectionVector = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(mDirectionVector) == 0) return;
	sf::Vector2f offset(VectorMath::normalizeVector(mDirectionVector));

	float moveX, moveY;

#ifdef _DESIGNER_HAX_
	moveX = offset.x*deltaTime.asSeconds()*SUPERMOVESPEED;
	moveY = offset.y*deltaTime.asSeconds()*SUPERMOVESPEED;
#endif // _DESIGNER_HAX_

	moveX = offset.x*deltaTime.asSeconds()*MOVESPEED;
	moveY = offset.y*deltaTime.asSeconds()*MOVESPEED;

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

void LuddisStatePlayable::attack(){
	// If the projectile is on cooldown, return
	if (mProjectileCooldown > 0) return;
	// If not, shoot

	// Get the current direction of luddis based on his frontvector and rotation
	sf::Vector2f direction = VectorMath::rotateVector(FRONTVECTOR, mPlayerPtr->getRotation());

	// Replace the current animation with an shooting animation and play a shooting sound
	mPlayerPtr->getAnimation()->replaceAnimation(SHOT_ANIMATION);
	// TODO Pull out constant variable
	SoundEngine::getInstance().playSound("Resources/Audio/Luddis_skott_16bit.wav");

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

void LuddisStatePlayable::updateRotation(){
	sf::Vector2f direction = getVectorMouseToSprite();
	if (VectorMath::getVectorLengthSq(direction) == 0) return;
	float rotation = VectorMath::getAngle(FRONTVECTOR, direction) * 180 / (float)M_PI;
	mPlayerPtr->setRotation(rotation);

	if ((direction.x <= 0 && !mIsFlipped) || (direction.x > 0 && mIsFlipped)) {
		mIsFlipped = !mIsFlipped;
	}
}

void LuddisStatePlayable::changeScale() {
	int dust = Inventory::getInstance().getDust();
	if (dust < 2 && mScale != sf::Vector2f(1.0f, 1.0f)) {
		mScale = { 1.0f , 1.0f };
		mPlayerPtr->getAnimation()->setDefaultAnimation(ANIMATION_ALMOSTDEAD);
	}
	else if (dust < 4 && dust > 1 && mScale != sf::Vector2f(1.10f, 1.10f)) {
		mScale = { 1.1f , 1.1f };
		mPlayerPtr->getAnimation()->setDefaultAnimation(ANIMATION_FILEPATH);
	}
	else if (dust < 6 && dust > 3 && mScale != sf::Vector2f(1.20f, 1.20f)) {
		mScale = { 1.2f , 1.2f };
	}
	else if (dust < 8 && dust > 5 && mScale != sf::Vector2f(1.30f, 1.30f)) {
		mScale = { 1.3f , 1.3f };
	}
	else if (dust > 7 && mScale != sf::Vector2f(1.4f, 1.4f)) {
		mScale = { 1.4f , 1.4f };
	}
	if (mIsFlipped == false)
		mPlayerPtr->setScale(mScale.y, mScale.y);
	if (mIsFlipped == true)
		mPlayerPtr->setScale(mScale.y, -mScale.y);

}

