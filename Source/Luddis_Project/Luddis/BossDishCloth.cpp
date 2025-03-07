#include "BossDishCloth.h"
#include "Projectile.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML\Graphics\Shape.hpp>
#include "ResourceManager.h"
#include "Inventory.h"
#include "GameStateLevel.h"
#include "GameStateMap.h"
#include "GameManager.h"
#include "Dialogue.h"
#include "SoundEngine.h"

//Different states depending on how damaged the boss is.
//State 1
static const std::string ANIMATION_IDLE = ("Resources/Images/Spritesheets/RagIdle_P1");
static const std::string SHOOTING_ANIMATION = ("Resources/Images/Spritesheets/RagAttack_P1");
//State 2
static const std::string ANIMATION_IDLE_2 = ("Resources/Images/Spritesheets/RagIdle_P2");
static const std::string SHOOTING_ANIMATION_2 = ("Resources/Images/Spritesheets/RagAttack_P2");
//State 3
static const std::string ANIMATION_IDLE_3 = ("Resources/Images/Spritesheets/RagIdle_P3");
static const std::string SHOOTING_ANIMATION_3 = ("Resources/Images/Spritesheets/RagAttack_P3");
//State 4
static const std::string ANIMATION_IDLE_4 = ("Resources/Images/Spritesheets/RagIdle_P4");
static const std::string SHOOTING_ANIMATION_4 = ("Resources/Images/Spritesheets/RagAttack_P4");

static const std::string PROJECTILE_FILEPATH = "Resources/Images/Rag_projectile.png";

static const std::string POWERUP1_FILEPATH = "Resources/Images/Rag_projectile.png";

static const std::string BOSS_START = "Resources/Configs/Dialogue/RagDialogue1.json";
static const std::string BOSS_DEFEATED = "Resources/Configs/Dialogue/RagDialogue2.json";

static const std::string ANIMATION_DEAD = "Resources/Images/Spritesheets/RagDead";
static const std::string ANIMATION_DEATH = "Resources/Images/Spritesheets/RagDeath";

static const int MAX_LIFE = 100;
static const float ATTACK_INTERVAL = 3.5f;
static const float ATTACK_TIME = 1.0f;
static const float ATTACK_FIRE_INTERVAL = 0.3f;
static const float PROJECTILE_LIFETIME = 2.5f;
static const float PROJECTILE_SPEED = 300;
static const float DEATH_TIMER = 1.5f;
static const sf::RectangleShape HITBOX_SHAPE = sf::RectangleShape(sf::Vector2f(225, 225));


void loadResources() {
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_2 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_2 + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_3 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_3 + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_IDLE_4 + ".png");
	ResourceManager::getInstance().loadTexture(SHOOTING_ANIMATION_4 + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_DEATH + ".png");
	ResourceManager::getInstance().loadTexture(ANIMATION_DEAD + ".png");
}

BossDishCloth::BossDishCloth(sf::RenderWindow* window, const sf::Vector2f& position, const float& activation, Transformable* aTarget, EntityManager* entityManager) :
	mIsAlive(true),
	mIsActive(false),
	mMeet(true),
	mWindow(window),
	mEntityManager(entityManager),
	mShooting(false),
	mActivate(activation),
	mLife(MAX_LIFE),
	mAttackInterval(ATTACK_INTERVAL),
	mGameStateLevel(&GameStateLevel::getInstance()),
	mDirection(0, 1.0f),
	mAnimation(Animation(ANIMATION_IDLE)),
	mHitbox(new sf::RectangleShape(HITBOX_SHAPE)),
	mTarget(aTarget),
	mDead(false),
	mCompleteLevel(false),
	mDeathTimer(DEATH_TIMER),
	mAttackTimer(ATTACK_TIME),
	mFireInterval(ATTACK_FIRE_INTERVAL)
{
	loadResources();
	setPosition(position);
	mHitbox->setOrigin(mHitbox->getLocalBounds().width / 2, mHitbox->getLocalBounds().height / 2);
}

BossDishCloth::~BossDishCloth() {
	delete mHitbox;
}

void BossDishCloth::tick(const sf::Time& deltaTime) {
	if (mDead)
		mDeathTimer -= deltaTime.asSeconds();

	if (mCompleteLevel) {
		GameStateMap::getInstance();
		GameManager::getInstance().setGameState(&GameStateMap::getInstance());
	}

	if (mTarget->getPosition().x >= mActivate) {
		mIsActive = true;
		if (mTarget->getPosition().x >= 11000){
			if (mMeet == true) {
				mGameStateLevel->createDialogue(BOSS_START);
				mMeet = false;
			}
		}
	}

	if (mTimeStunned <= 0) {
		//Move if alive and active
		if (mDead == false && mIsActive == true) {
			//Attack timer
			if (mAttackInterval <= 0) {
				mAttackTimer -= deltaTime.asSeconds();
				//Reload timer
				if (mFireInterval <= 0) {
					mFireInterval = ATTACK_FIRE_INTERVAL;
					attack();
					//End of attack phase
					if (mAttackTimer <= 0) {
						mAttackInterval = ATTACK_INTERVAL;
						mAttackTimer = ATTACK_TIME;
					}
				}
				else {
					mFireInterval -= deltaTime.asSeconds();
				}
			}
			//Move when not firing
			else {
				mAttackInterval -= deltaTime.asSeconds();
				updateMovement(deltaTime);
			}
		}
		mAnimation.tick(deltaTime);
	}
	else {
		mTimeStunned -= deltaTime.asSeconds();
	}

	if (mInvulnerable >= 0) {
		mInvulnerable -= deltaTime.asSeconds();
	}

	if (!mIsActive) return;

	if (mLife <= 0 && mDead == false) {
		mAnimation.setDefaultAnimation(ANIMATION_DEAD);
		mAnimation.overrideAnimation(ANIMATION_DEATH);
		mDead = true;
	}
	else if (mLife <= 0 && mDeathTimer <= 0) {
		mGameStateLevel->createDialogue(BOSS_DEFEATED);
		mCompleteLevel = true;
	}
}

void BossDishCloth::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mIsActive) return;
	states.transform *= getTransform();
	target.draw(mAnimation.getCurrAnimation(), states);
}

bool BossDishCloth::isAlive() const {
	return mIsAlive;
}

bool BossDishCloth::isActive() const {
	return mIsActive;
}

void BossDishCloth::setActive(const bool& active) {
	mIsActive = active;
}

Renderer::RenderLayer BossDishCloth::getRenderLayer() const {
	return Renderer::PLAYER;
}

void BossDishCloth::updateMovement(const sf::Time& deltaTime) {
	int spriteHeight = mAnimation.getCurrAnimation().getSprite().getTextureRect().height;
	if (getPosition().y < 0 + spriteHeight
		|| getPosition().y> mWindow->getView().getSize().y - spriteHeight) {
		mDirection = mDirection*-1.0f;
	}
	if (mAttackInterval > 0.6f &&
		mAttackInterval <= ATTACK_INTERVAL - 0.4f) {
		mShooting = false;
		move(mDirection);
	}
	else {
		if (!mShooting) {
			mShooting = true;
			//For different states of damage (changes happen next shot if hit when shooting)
			//State 1
			if (mLife < 26) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_4);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 2
			if (25 < mLife && mLife < 51) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_3);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 3
			if (50 < mLife && mLife < 76) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_2);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 4
			if (75 < mLife) {
				//int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION);
				//mAnimation.getCurrAnimation().setFrame(frame);
			}
			//mAnimation.replaceAnimation(SHOOTING_ANIMATION);
		}
	}
}

void BossDishCloth::attack() {
	sf::Vector2f vec(-1, 0);
	int max = 8;

	if (mLife < 50) {
		max = 2;
	}
	for (int i = 0; i < max; i++)
	{

		vec = VectorMath::rotateVector(vec, 360 / (float)max);
		Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, getPosition() + vec*PROJECTILE_SPEED / 3.0f, PROJECTILE_LIFETIME, 0, ENEMY_STUN);
		mEntityManager->addEntity(proj);
		CollisionManager::getInstance().addCollidable(proj);
	}

	Projectile* proj = new Projectile(PROJECTILE_FILEPATH, vec*PROJECTILE_SPEED, sf::Vector2f(getPosition().x, 590) + vec*PROJECTILE_SPEED / 3.0f, PROJECTILE_LIFETIME, 0, ENEMY_STUN);
	mEntityManager->addEntity(proj);
	CollisionManager::getInstance().addCollidable(proj);
}

BossDishCloth::Category BossDishCloth::getCollisionCategory() {
	if (mDead == true) {
		return SOLID;
	}
	else {
		return ENEMY_DAMAGE;
	}
}

BossDishCloth::Type BossDishCloth::getCollisionType() {
	return REC;
}

void BossDishCloth::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {
	if (mDead == true) {
		return;
	}
	else if (collidable->getCollisionCategory() == PLAYER_PROJECTILE) {
		if (!mShooting) {
			mLife -= 15;
			SoundEngine* se = &SoundEngine::getInstance();
			se->playEvent("event:/Gameplay/Luddis/Interaction/Luddis_Hit");
			// For different states of damages
			//State 1
			if (mLife < 26) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.setDefaultAnimation(ANIMATION_IDLE_4);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 2
			if (25 < mLife && mLife < 51) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.setDefaultAnimation(ANIMATION_IDLE_3);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 3
			if (50 < mLife && mLife < 76) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.setDefaultAnimation(ANIMATION_IDLE_2);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 4
			if (75 < mLife) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.setDefaultAnimation(ANIMATION_IDLE);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
		}
		if (mShooting) {
			//State 1
			if (mLife < 26) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_4);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 2
			if (25 < mLife && mLife < 51) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_3);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 3
			if (50 < mLife && mLife < 76) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION_2);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
			//State 4
			if (75 < mLife) {
				int  frame = mAnimation.getCurrAnimation().getCurrentFrame();
				mAnimation.replaceAnimation(SHOOTING_ANIMATION);
				mAnimation.getCurrAnimation().setFrame(frame);
			}
		}
	}
	else if (collidable->getCollisionCategory() == PLAYER_OBJECT) {
		if (mInvulnerable <= 0) {
			Inventory::getInstance().addDust(-1);
		}
	}
}

sf::FloatRect BossDishCloth::getHitBox() {
	return getTransform().transformRect(mAnimation.getCurrAnimation().getSprite().getGlobalBounds());
}
sf::Shape* BossDishCloth::getNarrowHitbox() const {
	mHitbox->setPosition(getPosition());
	mHitbox->setScale(getScale());
	mHitbox->setRotation(getRotation());
	return mHitbox;
}

int BossDishCloth::getCollisionDamage() const{
	return 0;
}

void BossDishCloth::stun(const sf::Time& deltatime) {
	mTimeStunned = float(deltatime.asSeconds());
}